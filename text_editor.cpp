/******************************************************************************
 * KIK-EDITOR: A minimal vi-like text editor.
 *
 * A simple text editor written in C++ to demonstrate terminal I/O,
 * raw mode processing, and basic text manipulation. It mimics core
 * functionalities of the `vi` editor, such as normal and insert modes.
 *
 * Features:
 * - Normal Mode (for navigation and commands)
 * - Insert Mode (for text entry)
 * - File I/O (opening, saving)
 * - Basic navigation (h, j, k, l)
 * - Basic editing (x for delete, o for new line)
 * - Command mode (':') for saving (:w) and quitting (:q, :wq)
 *
 * Compilation:
 * g++ -std=c++11 -o kik-editor kik-editor.cpp
 *
 * Usage:
 * ./kik-editor [filename]
 *
 ******************************************************************************/
#include <cstdio>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <stdexcept>
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <thread>
#include <chrono>
// POSIX API headers
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>

// --- Defines ---
#define KIK_VERSION "1.0"
#define CTRL_KEY(k) ((k) & 0x1f)

// --- Data Structures ---

// Enum for editor modes
enum EditorMode {
    NORMAL,
    INSERT,
    COMMAND
};

// Global struct to hold the editor's state
struct EditorState {
    int cx, cy;             // Cursor position within the file buffer
    int screen_rows;
    int screen_cols;
    int row_offset;
    int col_offset;    // Vertical scroll position
    std::vector<std::string> lines; // File content, one string per line
    EditorMode mode;
    std::string status_msg;
    std::string filename;
    bool dirty;             // True if there are unsaved changes
    struct termios orig_termios;
};

EditorState E;

// --- Function Prototypes ---
void enableRawMode();
void disableRawMode();
void die(const std::string& s);
char editorReadKey();
void editorProcessKeypress();
void editorRefreshScreen();
void editorDrawRows(std::string& buffer);
void editorDrawStatusBar(std::string& buffer);
void editorOpen(const char* filename);
void editorSave();
std::string editorPrompt(const std::string& prompt);

// --- Terminal Control ---

/**
 * @brief Prints an error message and exits the program.
 * @param s The error message to display.
 */
void die(const std::string& s) {
    write(STDOUT_FILENO, "\x1b[2J", 4);
    write(STDOUT_FILENO, "\x1b[H", 3);
    perror(s.c_str());
    exit(1);
}

/**
 * @brief Restores the terminal to its original settings.
 */
void disableRawMode() {
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &E.orig_termios) == -1) {
        die("tcsetattr failed");
    }
}

/**
 * @brief Enables raw mode for the terminal.
 * Raw mode allows reading input character-by-character without waiting for Enter.
 */
void enableRawMode() {
    if (tcgetattr(STDIN_FILENO, &E.orig_termios) == -1) die("tcgetattr failed");
    atexit(disableRawMode);

    struct termios raw = E.orig_termios;
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    raw.c_oflag &= ~(OPOST);
    raw.c_cflag |= (CS8);
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr failed");
}

/**
 * @brief Reads a single keypress from standard input.
 * @return The character that was read.
 */
char editorReadKey() {
    int nread;
    char c;
    while ((nread = read(STDIN_FILENO, &c, 1)) != 1) {
        if (nread == -1 && errno != EAGAIN) die("read failed");
    }
    return c;
}

/**
 * @brief Gets the terminal window size.
 * @param rows Pointer to store the number of rows.
 * @param cols Pointer to store the number of columns.
 * @return 0 on success, -1 on failure.
 */
int getWindowSize(int& rows, int& cols) {
    struct winsize ws;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
        return -1;
    }
    cols = ws.ws_col;
    rows = ws.ws_row;
    return 0;
}

// --- Editor Operations ---

/**
 * @brief Initializes the editor state.
 */
void initEditor() {
    E.cx = 0;
    E.cy = 0;
    E.row_offset = 0;
    E.col_offset=0;
    E.mode = NORMAL;
    E.status_msg = "HELP: :q = quit | :w = save | :wq = save & quit";
    E.filename = "[No Name]";
    E.dirty = false;

    if (getWindowSize(E.screen_rows, E.screen_cols) == -1) die("getWindowSize failed");
    E.screen_rows -= 1; // For the status bar
}

/**
 * @brief Moves the cursor based on keyboard input.
 * @param key The key pressed ('h', 'j', 'k', 'l').
 */
void editorMoveCursor(char key) {
    switch (key) {
        case 'h': if (E.cx > 0) E.cx--; break;
        case 'l': if (E.cy < E.lines.size() && E.cx < E.lines[E.cy].length()) E.cx++; break;
        case 'k': if (E.cy > 0) E.cy--; break;
        case 'j': if (E.cy < E.lines.size() - 1) E.cy++; break;
    }
    // Snap cursor to end of line if needed
    if (E.cy < E.lines.size() && E.cx > E.lines[E.cy].length()) {
        E.cx = E.lines[E.cy].length();
    }
}

/**
 * @brief Inserts a character at the cursor's position.
 * @param c The character to insert.
 */
void editorInsertChar(char c) {
    if (E.cy == E.lines.size()) {
        E.lines.push_back("");
    }
    E.lines[E.cy].insert(E.cx, 1, c);
    E.cx++;
    E.dirty = true;
}

/**
 * @brief Deletes the character at the cursor's position.
 */
void editorDeleteChar() {
    if (E.cy >= E.lines.size()) return;
    if (E.cx > 0) {
        E.lines[E.cy].erase(E.cx - 1, 1);
        E.cx--;
        E.dirty = true;
    }
}

/**
 * @brief Processes keypresses based on the current editor mode.
 */
void editorProcessKeypress() {
    char c = editorReadKey();

    if (E.mode == COMMAND) {
        // Command mode logic is handled within editorPrompt
        return;
    }

    if (E.mode == INSERT) {
        switch (c) {
            case '\x1b': // Escape key
                E.mode = NORMAL;
                E.status_msg = "NORMAL MODE";
                break;
            case 127: // Backspace
                editorDeleteChar();
                break;
            case '\r': // Enter
                 if (E.cy == E.lines.size()) {
                    E.lines.push_back("");
                } else {
                    E.lines.insert(E.lines.begin() + E.cy + 1, E.lines[E.cy].substr(E.cx));
                    E.lines[E.cy] = E.lines[E.cy].substr(0, E.cx);
                }
                E.cy++;
                E.cx = 0;
                E.dirty = true;
                break;
            default:
                editorInsertChar(c);
                break;
        }
    } else if (E.mode == NORMAL) {
        switch (c) {
            case 'i':
                E.mode = INSERT;
                E.status_msg = "INSERT MODE";
                break;
            case 'h':
            case 'j':
            case 'k':
            case 'l':
                editorMoveCursor(c);
                break;
            case 'x':
                if (E.cy < E.lines.size() && E.cx < E.lines[E.cy].length()) {
                    E.lines[E.cy].erase(E.cx, 1);
                    E.dirty = true;
                }
                break;
            case 'o':
                E.cy++;
                E.lines.insert(E.lines.begin() + E.cy, "");
                E.cx = 0;
                E.dirty = true;
                E.mode = INSERT;
                E.status_msg = "INSERT MODE";
                break;
            case ':': {
                std::string cmd = editorPrompt(":");
                if (!cmd.empty()) {
                    if (cmd == "q") {
                        if (E.dirty) {
                            E.status_msg = "Unsaved changes! Use :q! to force quit.";
                        } else {
                            write(STDOUT_FILENO, "\x1b[2J", 4);
                            write(STDOUT_FILENO, "\x1b[H", 3);
                            exit(0);
                        }
                    } else if (cmd == "q!") {
                        write(STDOUT_FILENO, "\x1b[2J", 4);
                        write(STDOUT_FILENO, "\x1b[H", 3);
                        exit(0);
                    }
                    else if (cmd == "w") {
                        editorSave();
                    } else if (cmd == "wq") {
                        editorSave();
                        write(STDOUT_FILENO, "\x1b[2J", 4);
                        write(STDOUT_FILENO, "\x1b[H", 3);
                        exit(0);
                    } else {
                        E.status_msg = "Unknown command: " + cmd;
                    }
                }
                break;
            }
        }
    }
}


// --- Screen Output ---

/**
 * @brief Handles scrolling of the viewport.
 */
// REPLACE THE OLD editorScroll FUNCTION WITH THIS:
void editorScroll() {
    // Vertical scrolling
    if (E.cy < E.row_offset) {
        E.row_offset = E.cy;
    }
    if (E.cy >= E.row_offset + E.screen_rows) {
        E.row_offset = E.cy - E.screen_rows + 1;
    }
    // Horizontal scrolling
    if (E.cx < E.col_offset) {
        E.col_offset = E.cx;
    }
    if (E.cx >= E.col_offset + E.screen_cols) {
        E.col_offset = E.cx - E.screen_cols + 1;
    }
}

/**
 * @brief Draws the text rows to the screen buffer.
 * @param buffer The string buffer to append drawing commands to.
 */
// REPLACE THE OLD editorDrawRows FUNCTION WITH THIS:
void editorDrawRows(std::string& buffer) {
    for (int y = 0; y < E.screen_rows; y++) {
        int file_row = y + E.row_offset;
        if (file_row >= E.lines.size()) {
            buffer.append("~\r\n");
        } else {
            std::string line = E.lines[file_row];
            if (line.length() > E.col_offset) {
                line = line.substr(E.col_offset);
            } else {
                line = "";
            }
            if (line.length() > E.screen_cols) {
                line = line.substr(0, E.screen_cols);
            }
            buffer.append(line);
            buffer.append("\r\n");
        }
    }
}

/**
 * @brief Draws the status bar at the bottom of the screen.
 * @param buffer The string buffer to append drawing commands to.
 */
void editorDrawStatusBar(std::string& buffer) {
    buffer.append("\x1b[7m"); // Invert colors
    std::string status = E.filename + (E.dirty ? " [Modified]" : "") + " - " + std::to_string(E.lines.size()) + " lines";
    std::string pos = std::to_string(E.cy + 1) + ":" + std::to_string(E.cx + 1);
    
    buffer.append(status);
    int len = status.length();
    while (len < E.screen_cols - pos.length()) {
        buffer.append(" ");
        len++;
    }
    buffer.append(pos);
    buffer.append("\x1b[m"); // Reset colors
    buffer.append("\r\n");
    buffer.append(E.status_msg);
}

/**
 * @brief Refreshes the entire screen with the current editor state.
 */
// REPLACE THE OLD editorRefreshScreen FUNCTION WITH THIS:
void editorRefreshScreen() {
    editorScroll();

    // Force clear the screen and move cursor to top-left
    write(STDOUT_FILENO, "\x1b[2J", 4);
    write(STDOUT_FILENO, "\x1b[H", 3);

    std::string buffer;
    editorDrawRows(buffer);
    editorDrawStatusBar(buffer);

    // Position cursor relative to the scroll offset
    int cursor_y = E.cy - E.row_offset + 1;
    int cursor_x = E.cx - E.col_offset + 1;
    buffer.append("\x1b[" + std::to_string(cursor_y) + ";" + std::to_string(cursor_x) + "H");

    write(STDOUT_FILENO, buffer.c_str(), buffer.length());
}


// --- File I/O ---

/**
 * @brief Reads a file from disk into the editor buffer.
 * @param filename The name of the file to open.
 */
void editorOpen(const char* filename) {
    E.filename = filename;
    std::ifstream file(filename);
    if (file.is_open()) {
        std::string line;
        while (getline(file, line)) {
            // Strip trailing carriage return/newline characters
            while (!line.empty() && (line.back() == '\r' || line.back() == '\n')) {
                line.pop_back();
            }
            E.lines.push_back(line);
        }
        file.close();
    }
}

/**
 * @brief Saves the current buffer to disk.
 */
void editorSave() {
    if (E.filename == "[No Name]") {
        E.filename = editorPrompt("Save as: ");
        if (E.filename.empty()) {
            E.status_msg = "Save aborted.";
            return;
        }
    }

    std::ofstream file(E.filename);
    if (file.is_open()) {
        int len = 0;
        for (const auto& line : E.lines) {
            file << line << std::endl;
            len += line.length() + 1;
        }
        file.close();
        E.dirty = false;
        E.status_msg = std::to_string(len) + " bytes written to " + E.filename;
    } else {
        E.status_msg = "Error writing to file: " + std::string(strerror(errno));
    }
}

/**
 * @brief Prompts the user for input in the status bar.
 * @param prompt The prompt to display.
 * @return The user's input string.
 */
std::string editorPrompt(const std::string& prompt) {
    std::string input;
    E.mode = COMMAND;
    while (true) {
        E.status_msg = prompt + input;
        editorRefreshScreen();
        char c = editorReadKey();
        if (c == '\r') {
            E.mode = NORMAL;
            E.status_msg = "";
            return input;
        } else if (c == '\x1b') { // Escape
            E.mode = NORMAL;
            E.status_msg = "";
            return "";
        } else if (c == 127 && !input.empty()) { // Backspace
            input.pop_back();
        } else if (!iscntrl(c)) {
            input += c;
        }
    }
}

// --- Main ---

int main(int argc, char* argv[]) {
    enableRawMode();
    initEditor();
    if (argc >= 2) {
        editorOpen(argv[1]);
    }

   while (true) {
    editorRefreshScreen();
    editorProcessKeypress();
    // Add a 16-millisecond delay to yield time to the terminal renderer.
    // This is roughly equivalent to one frame at 60 FPS.
    //  std::this_thread::sleep_for(std::chrono::milliseconds(16)); // <-- ADD THIS LINE
}


    return 0;
}
