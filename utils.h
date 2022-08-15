// Author - Aman-Rohilla@rohilla.co.in

#ifndef UTILS__H
#define UTILS__H

#define _WIN32_WINNT 0x0600
#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#pragma comment(lib, "Wininet.lib")
#include <wininet.h>
#include <fstream>
#include <future>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <conio.h>

#include <shlwapi.h>
#include <iomanip>
#include <iterator>
#include <sstream>
#include <windows.h>
#include <jsoncpp/json.h>
#include <jsoncpp/reader.h>
#include <jsoncpp/writer.h>
#include <jsoncpp/value.h>
#include <math.h>

#define lpn string::npos
using namespace std;

HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);
int argcg;
char **argvg;

bool str_find(string s, string str, size_t i = 0)
{
    if (s.find(str, i) != string::npos)
        return 1;

    return 0;
}

size_t str_find_index(string &s, string str, size_t i = 0, int reverse = 0)
{
    size_t f;
    if (!reverse)
        f = s.find(str, i);
    else
        f = s.rfind(str, i);
    if (f != string::npos)
        return f;

    return -1;
}
int str_find_count(string s, string str, size_t f = 0)
{
    f = s.find(str, f);
    if (f == string::npos)
        return 0;
    int count = 0;
    while (f != string::npos)
    {
        count++;
        f = s.find(str, f + 1);
    }
    return count;
}

string erase_find(string s, string str)
{
    size_t f = s.find(str);
    if (f != string::npos)
        s.erase(0, f + 1);
    return s;
}

string erase_find_found_to_end(string s, string str)
{
    size_t f = s.find(str);
    if (f != string::npos)
        s.erase(f, s.length() - f);

    return s;
}

string erase_rfind(string s, string str)
{
    size_t f = s.rfind(str);
    if (f != string::npos)
        s.erase(0, f + 1);
    return s;
}

string erase_rfind_found_to_end(string s, string str)
{
    size_t f = s.rfind(str);
    if (f != string::npos)
        s.erase(f, s.length() - f);

    return s;
}

COORD GetConsoleCursorPosition(HANDLE hConsoleOutput)
{
    CONSOLE_SCREEN_BUFFER_INFO cbsi;
    if (GetConsoleScreenBufferInfo(hConsoleOutput, &cbsi))
    {
        return cbsi.dwCursorPosition;
    }
    else
    {
        COORD invalid = {0, 0};
        return invalid;
    }
}

int current_console_cursor(int &x, int &y, int set = 0)
{
    COORD c;
    if (!set)
    {
        c = GetConsoleCursorPosition(hstdout);
        x = c.Y;
        y = c.X;
        return 1;
    }
    else
    {
        c.X = y;
        c.Y = x;
        bool b = SetConsoleCursorPosition(hstdout, c);
        return b;
    }
}

inline void setConsoleColor(WORD c)
{
    SetConsoleTextAttribute(hstdout, c);
}
string add_gaps(string s, int n = 0, int front = 0, string gap = " ")
{
    int gaps = n - s.length();
    for (int i = 0; i < gaps; i++)
    {
        if (front)
            s = gap + s;
        else
            s += gap;
    }
    return s;
}

void console_cursor(bool show)
{
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hstdout, &cursorInfo);
    cursorInfo.bVisible = show;
    SetConsoleCursorInfo(hstdout, &cursorInfo);
}

bool exists(string s)
{
    if (s == "")
        return 0;
    return PathFileExistsA(s.c_str());
}

string del_ex(string in)
{
    string s = in;
    size_t f = s.rfind(".");
    if (f == string::npos)
        return s;

    s.erase(f, s.length() - f);
    return s;
}

string ext_ex(string in, int dot = 1)
{
    string s = in;
    size_t f = s.rfind(".");
    if (f == string::npos)
        return "";

    if (dot)
        s.erase(0, f);
    else
        s.erase(0, f + 1);
    return s;
}

string ext_dn(string s)
{
    size_t f = s.rfind("\\");
    if (f != string::npos)
        s.erase(f + 1, s.length() - f - 1);
    else
        s = "";
    return s;
}
string ext_fn(string s)
{
    size_t f = s.rfind(".");
    if(f!=string::npos) s.erase(f,s.length()-f);

    f = s.rfind("\\");
    if(f!=string::npos) s.erase(0,f+1);
    return s;
}

string ext_fnwe(string s)
{
    size_t f = s.rfind("\\");
    if(f!=string::npos) s.erase(0,f+1);
    return s;
}

string exe_path()
{
    char result[MAX_PATH];
    return string(result, GetModuleFileNameA(NULL, result, MAX_PATH));
}

string out__file(string s, string app)
{
    return del_ex(s) + app + ext_ex(s);
}

string name_assigner(string s)
{
    if (!exists(s))
        return s;

    int i = 2;
    while (exists(out__file(s, "_" + to_string(i))))
        i++;
    return out__file(s, "_" + to_string(i));
}

string replace_string_all(string s, string old, string new_s, int total = 0)
{
    size_t f = s.find(old);
    while (f != string::npos)
    {
        s.replace(f, old.length(), new_s);
        if (!total)
            f = s.find(old, f + new_s.length());
        else
            f = s.find(old);
    }
    return s;
}

string validate_path(string s)
{
    // Invalid chars <>:"/\|?*
    // integer value 127
    // Integer value 0 to 32, -256 to -224

    if (s.length() == 0)
        return "";

    s = replace_string_all(s, "\n", "_");
    s = replace_string_all(s, "\r", "_");

    int len = s.length();
    vector<int> arr = {-256, -255, -254, -253, -252, -251, -250, -249, -248, -247, -246, -245, -244, -243,
                       -242, -241, -240, -239, -238, -237, -236, -235, -234, -233, -232, -231, -230, -229,
                       -228, -227, -226, -225, -224, -222, -214, -209, -198, -196, -194, -193, -164, -132,
                       -129, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23,
                       24, 25, 26, 27, 28, 29, 30, 31, 34, 42, 47, 58, 60, 62, 63, 124, 127};

    // char chars[] = {'<','>',':','"','/','\\','|','?','*'};
    for (int i = 0; i < len; i++)
        if (binary_search(arr.begin(), arr.end(), (int)s[i]))
        {
            if (s[i] == ':' && s[1] == ':' && s[2] == '\\')
                continue;
            s.replace(i, 1, "_");
        }

    len = s.length();

    if (s[len - 1] == ' ' || s[len - 1] == '.')
        s.replace(len - 1, 1, "_");

    return s;
}

string lower_case_converter(string s)
{
    int len = s.length();
    if (!len)
        return s;

    for (int i = 0; i < len; i++)
        if (s[i] <= 'Z' && s[i] >= 'A')
            s[i] = (char)((int)s[i] + 32);

    return s;
}


string text_to_string(string file)
{
    if (!exists(file))
        return "";
    string s;

    ifstream text(file);
    stringstream ss;
    ss << text.rdbuf();
    text.close();
    s = ss.str();
    return s;
}

int lines_of_text(string s, int f = 1)
{
    ifstream in(s);
    if (!in)
        return -1;
    int lines = 0;

    while (!in.eof())
    {
        getline(in, s);
        if (f == 1 && s.length() == 0)
            continue;
        lines++;
    }
    return lines;
}

template <typename T>
void write_to_text(string file, T s, int append = 1, int new_line = 1)
{
    int lines = lines_of_text(file);
    if (lines == -1)
        lines = 0;

    if (append)
    {
        ofstream out(file, ios_base::app | ios_base::ate);
        if (new_line && lines)
            out << endl;
        out << s;
        out.close();
    }
    else
    {
        ofstream out(file);
        out << s;
        out.close();
    }
    return;
}

string pipe_open(string s, int *retval = NULL, int std_err = 0, int capture = 1, int std_err_nul = 1, string *error = NULL)
{
    char buffer[4096];
    FILE *pipe;

    string tf;

    if (std_err_nul)
        s += " 2>nul";
    else if (std_err && !error)
        s += " 2>&1";
    else if (std_err && error)
    {
        tf = name_assigner("temp");
        s += " 2>" + tf;
    }

    if ((pipe = _popen(s.c_str(), "rt")) == NULL)
    {
        if (retval)
            *retval = -1;
        return "";
    }

    string str;
    while (fgets(buffer, 4096, pipe))
    {
        if (capture)
            str += buffer;
    }

    if (feof(pipe) && retval)
        *retval = _pclose(pipe);

    int i = 1;
    if (feof(pipe))
        i = 1;
    if (tf != "")
        *error = text_to_string(tf);

    return str;
}

int str2json(string &str, Json::Value &root, string *errors = NULL)
{
    string js;
    Json::CharReaderBuilder builder;
    Json::CharReader *reader = builder.newCharReader();
    bool b = reader->parse(str.c_str(), str.c_str() + str.length(), &root, errors);
    delete reader;

    return b;
}

string get_column(string s, int col, char c = '|')
{
    if (col < 0 || s == "")
        return "";
    if(col==0) return s;

    stringstream ss = stringstream(s);

    int i = 0;
    while (!ss.eof())
    {
        i++;
        getline(ss, s, c);
        if (i == col)
            return s;
    }

    return "";
}

string write_column(string s, int col, string col_str, int n_cols=0)
{
    if(s.length()==0)
    for(int i=0; i<n_cols-1; i++)
    s.push_back('|');

    if(col<=0)  return s;
    stringstream ss = stringstream(s);
    string ret_str;
    int i=0, flag=0;
    while(!ss.eof())
    {
        i++;
        getline(ss,s,'|');
        if(flag==1) ret_str += "|";
        flag=1;

        if(i==col)  ret_str += col_str;
        else        ret_str += s;
    }
    return ret_str;
}


void console_pointer_code(string s)
{
    if (s.length() == 0)
        return;

    DWORD mode = 0;
    if (!GetConsoleMode(hstdout, &mode))
        return;

    const DWORD original_mode = mode;
    mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;

    if (!SetConsoleMode(hstdout, mode))
    {
        return;
    }

    // Write the sequence for clearing the display.
    DWORD written = 0;

    string cmd;

    if (s == "up")
        cmd = "\033[A";
    else if (s == "b")
        cmd = "\b";
    else if (s == "el")
        cmd = "\33[2K";
    else if (s == "els")
        cmd = "\33[2K\r";
    else if (s == "sl")
        cmd = "\r";
    else if (s == "cl")
        cmd = "\33[2J";

    PCWSTR str = (std::wstring(cmd.begin(), cmd.end())).c_str();

    if (cmd != "" && !WriteConsoleW(hstdout, str, (DWORD)wcslen(str), &written, NULL))
    {
        SetConsoleMode(hstdout, original_mode);
        return;
    }

    SetConsoleMode(hstdout, original_mode);
}

int is_int(string s)
{
    int len = s.length();
    if (!len)
        return 0;
    for (int i = 0; i < len; i++)
        if (s[i] < '0' || s[i] > '9')
            return 0;
    return 1;
}

int s2i(string s, int *n)
{
    if (is_int(s))
    {
        if (n)
            *n = stoi(s);
        return 1;
    }

    return 0;
}
void console_pointer(string s, int skip = -1);

void erase_console_line(int X, int skip = -1)
{
    if (X < 0)
        return;
    int x, y;

    current_console_cursor(x, y);
    while (x >= X)
    {
        current_console_cursor(x, y, 1);
        if (skip != x)
            console_pointer("el");
        x--;
    }
}

void console_pointer(string s, int skip)
{
    if (!s.length())
        return;
    s = replace_string_all(s, "  ", " ");

    stringstream ss = stringstream(s);

    while (!ss.eof())
    {
        getline(ss, s, ' ');
        if (s == "ml")
        {
            getline(ss, s, ' ');
            int left;

            if (s2i(s, &left))
            {
                int x, y;
                current_console_cursor(x, y);
                y = y - left;
                if (y < 0)
                    y = 0;
                current_console_cursor(x, y, 1);
            }
        }
        else if (s == "mr")
        {
            getline(ss, s, ' ');
            int right;
            if (s2i(s, &right))
            {
                int x, y;
                current_console_cursor(x, y);
                y = y + right;
                current_console_cursor(x, y, 1);
            }
        }
        else if (s == "mu")
        {
            getline(ss, s, ' ');
            int up;
            if (s2i(s, &up))
            {
                int x, y;
                current_console_cursor(x, y);
                x = x - up;
                if (x < 0)
                    x = 0;
                current_console_cursor(x, y, 1);
            }
        }
        else if (s == "md")
        {
            getline(ss, s, ' ');
            int down;
            if (s2i(s, &down))
            {
                int x, y;
                current_console_cursor(x, y);
                x = x + down;
                current_console_cursor(x, y, 1);
            }
        }
        else if (s == "cl")
        {
            erase_console_line(0);
        }
        else if (s == "etl")
        {
            getline(ss, s, ' ');
            int x;
            if (s2i(s, &x))
                erase_console_line(x, skip);
        }
        else if (s == "etls")
        {
            getline(ss, s, ' ');
            int x;
            if (s2i(s, &x))
                erase_console_line(x, skip);
            console_pointer("sl");
        }
        else if (s == "st")
        {
            int x = 0, y = 0;
            current_console_cursor(x, y, 1);
        }
        else if (s == "ess")
        {
            console_pointer("cl st");
        }
        else if (s == "sp")
        {
            int x, y;
            getline(ss, s, ' ');

            if (s2i(s, &x))
            {
                getline(ss, s, ' ');
                if (s2i(s, &y))
                    current_console_cursor(x, y, 1);
            }
        }
        else
            console_pointer_code(s);
    }
}

int get_console_rc(int col = 1)
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hstdout, &csbi);
    if (col)
        return csbi.srWindow.Right - csbi.srWindow.Left + 1;
    else
        return csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
}

void erase_console(int X, int Y, int skip = -1)
{
    if (X < 0 || Y < 0)
        return;
    int x, y;

    current_console_cursor(x, y);
    while (x >= X + 1)
    {
        current_console_cursor(x, y, 1);
        if (skip != x)
            console_pointer("el");
        x--;
    }
    int len = get_console_rc() - Y;

    string s;
    for (int i = 0; i < len; i++)
        s += " ";

    current_console_cursor(X, Y, 1);
    cout << s;
    current_console_cursor(X, Y, 1);
}

struct codepage__
{
    int cp;
    codepage__()
    {
        cp = GetConsoleOutputCP();
    }
    void unicode()
    {
        SetConsoleOutputCP(65001);
    }
    void restore()
    {
        SetConsoleOutputCP(cp);
    }
};

struct progress_bar_param
{
    int execute = 1;
    int process = 0;
    int perform = 0;
    std::future<int> rv;
    int current_count = 0;
    int total_count = 0;
    int current = 0;
    int started = 0;
    int max_len = 0;
    int prog_start_x = 0;
    int start_x = 0;
    int start_y = 0;
    int start_X = 0;
    int start_Y = 0;
    int end_x = 0;
    int end_y = 0;
    int end_X = 0;
    int end_Y = 0;
    int once = 0;
    int new_line_len = 0;
    int new_line_x = 0;
    int new_line_y = 0;
    int info_len = 0;
    int mode = 0;
    int async = 1;
    int active = 0;
    int exit = 0;
    int running = 0;
    codepage__ cp;
    /// above variables are internal to progress bar

    string task;
    string extra_info;
    string new_line;

    int precision = 0;
    int set_len = 0;
    int clear = 1;
    int percentage_gap = 1;
    int resize = 1;
    int display = 1;
    int cursor = 0;
    int console_size = 0;
    int console_height = 0;
    int wide_chars = 1;
    int bell = 0;
    int pause = 0;
    int paused = 0;
    int organize = 1;
    int ended = 1;
    int first_line = 1;
    int thread = 0;
    int delay = 50;
    int delay_max = 500;
    int delay_min = 20;
    int save_state = 0;
    int delay_clear = 1000;
    int cleared = 0;
    int blocks = 50;
    float interval = 1;
    int ran = 0;

    string completed = "█";
    string remaining = "░";

    string tab = "        ";
    string start_lines;
    string task_lines = "\n\n";
    string new_lines = "\n\n";
    string end_lines = "\n\n";
    string transition = " -> ";

    progress_bar_param(string task_ = "");

    void launch();
    void relaunch();
    void end();
    void update(int comp = 0, int thread_ = 0);
    void terminate(string s = "");
};
int progress_bar_async(progress_bar_param *pb);
void progress_bar(progress_bar_param &pb, int comp = 0);

progress_bar_param::progress_bar_param(string task_)
{
    int y;
    current_console_cursor(this->prog_start_x, y);
    task = task_;
}

void progress_bar_param::relaunch()
{
    if (!this->execute)
        return;
    this->terminate();
    this->launch();
}

void progress_bar_param::end()
{
    cout.flush();
    if (!this->execute)
        return;

    if (!this->ended && !this->save_state)
    {
        if (this->wide_chars)
        {
            this->cp.restore();
            this->mode = 0;
        }

        if (this->new_line_x > this->start_x || (this->new_line_x == this->start_x && this->new_line_y > this->start_y))
            current_console_cursor(this->new_line_x, this->new_line_y, 1);

        if (this->clear && this->started)
        {
            Sleep(this->delay_clear);
            erase_console(this->start_X, this->start_Y);
            this->end_X = this->start_X;
            this->end_Y = this->start_Y;
        }
        else
            cout << this->end_lines;

        if (this->clear)
            this->once = 0;
        else
            this->once = 1;
        console_cursor(1);
        if (this->bell)
            cout << "\a";

        current_console_cursor(this->end_X, this->end_Y);

        this->extra_info = "";
        this->new_line = "";

        this->current_count = 0;
        this->total_count = 0;
        this->current = 0;
        this->started = 0;
        this->new_line_len = 0;
        this->info_len = 0;
        this->process = 0;
        this->active = 0;
        this->cleared = 0;
        this->ended = 1;
    }
}

void progress_bar_param::terminate(string s)
{
    cout.flush();

    if (this->ran)
    {
        this->ran = 0;
        if (!this->ended)
            this->update(1);
    }

    if (this->running)
    {
        this->exit = 1;
        while (this->running)
            Sleep(this->delay);
    }
    else
        this->end();

    if (s == "up")
    {
        if (this->organize)
        {
            int x, y;
            current_console_cursor(x, y);
            if (!this->clear && x == this->end_X && y == this->end_Y && this->end_lines.find("\n\n") != string::npos)
                console_pointer("up");
        }
    }
}

void progress_bar_param::launch()
{
    if (!this->execute || !this->thread)
        return;
    if (!this->running)
    {
        this->rv = std::async(std::launch::async, progress_bar_async, this);
        while (!this->running)
            Sleep(this->delay);
    }
}

void progress_bar_param::update(int comp, int thread_)
{
    cout.flush();
    if (!this->execute)
        return;
    if (!this->ran)
        ran = 1;

    if (this->running)
    {
        if (comp)
            this->perform = 1;
        else
            this->perform = 0;
        this->process++;
        return;
    }

    if (thread_)
        thread = 1;

    if (this->thread)
    {
        this->launch();
        if (comp)
            this->perform = 1;
        else
            this->perform = 0;
        this->process++;
    }
    else
        progress_bar(*this, comp);
}

string fill_gap(string s, int n)
{
    int len = s.length();
    for (int i = 0; i < (n - len); i++)
        s.push_back(' ');
    return s;
}
int no_console_gaps(int a1, int b1, int a2, int b2, int size)
{
    if (a1 == a2 && b1 == b2)
        return 0;
    if (size == 0)
        return 0;

    int gaps = 0;

    int x = abs(a2 - a1);
    int i = 0;

    while (i < x)
    {
        gaps += size;
        i++;
    }
    gaps += abs(b2 - b1);
    return gaps;
}

void progress_bar(progress_bar_param &pb, int comp)
{
    if (!pb.execute)
        return;
    if (!pb.total_count || !pb.execute)
        return;
    int p;
    string s, pbcs, pbrs;
    string info;

    pbcs = pb.completed;
    pbrs = pb.remaining;
    int pbcc = pb.current_count;
    if (pbcc && !comp)
        pbcc--;

    float pp = ((float)(pbcc * 100)) / pb.total_count;
    if (pp > 100)
        pp = 100;
    p = (int)pp;
    if (pp - p > 0.5 && p != 99)
        p++;

    if (!pb.started)
    {
        pb.end();
        pb.ended = 0;

        string pbbc[] = {"▌", "█"};

        sort(pbbc, pbbc + 2);

        if (binary_search(pbbc, pbbc + 2, pb.completed))
        {
            pb.completed = "█";
            pbcs = pb.completed;
        }
        pb.max_len = (pb.task + to_string(pb.total_count) + "/" + to_string(pb.total_count)).length();

        if (pb.wide_chars)
        {
            pb.cp.unicode();
            pb.mode = 1;
        }

        int x, y;
        pb.current = -1;
        current_console_cursor(x, y);
        pb.start_X = x;
        pb.start_Y = y;

        if (pb.organize && x == pb.prog_start_x && pb.first_line)
        {
            cout << endl;
            x++;

            if (y)
            {
                cout << endl;
                x++;
            }
            y = 0;
        }
        else if (pb.organize && x && pb.end_X)
        {
            if (x == pb.end_X && !y)
            {
                x++;
                cout << endl;
                y = 0;
            }
            else if (x == pb.end_X || y)
            {
                cout << endl
                     << endl;
                x += 2;
                y = 0;
            }
        }

        pb.start_x = x;
        pb.start_y = y;
        if (!pb.cursor)
            console_cursor(0);
    }

    if (pb.current_count <= pb.current && pb.task == "" && pb.new_line == "" && pb.extra_info == "" && !comp)
        return;

    float per = 100.0 / pb.blocks;
    int chars = pp / per;

    for (int k = 0; k < chars; k++)
        s += pbcs;

    if (pb.completed == "█")
    {
        float fx = fmod(pp, per);
        if (fx >= per / 2)
        {
            chars++;
            s += "▌";
        }
    }

    for (int i = 0; i < pb.blocks - chars; i++)
        s += pbrs;

    s = pb.task_lines + pb.tab + s + " ";
    if (p < 10)
        s += "  ";
    else if (p < 100)
        s += " ";
    s += to_string(p);

    if (pb.percentage_gap)
        s += " ";
    s = s + "%";

    int console_cols = get_console_rc();

    if (pb.cleared == 1)
    {
        erase_console(pb.start_x, pb.start_y);
        pb.cleared = 0;
    }

    current_console_cursor(pb.start_x, pb.start_y, 1);

    if (pb.task != "" || pb.extra_info != "")
    {
        if (pb.task != "" && pb.extra_info != "")
        {
            if (!pb.set_len)
                info = fill_gap(pb.task + to_string(pb.current_count) + "/" + to_string(pb.total_count), pb.max_len) + pb.extra_info;
            else
                info = fill_gap(pb.task + to_string(pb.current_count) + "/" + to_string(pb.total_count), pb.set_len) + pb.extra_info;
        }
        else if (pb.task != "")
            info = pb.task + to_string(pb.current_count) + "/" + to_string(pb.total_count);
        else if (pb.extra_info != "")
            info = pb.extra_info;

        info = replace_string_all(info, "\t", pb.tab);
        info = replace_string_all(info, "\n", "");

        int last_info_len = pb.info_len;

        pb.info_len = info.length();

        if (info.length() > console_cols)
            info.erase(console_cols, info.length() - console_cols);
        else if (pb.started && last_info_len > pb.info_len)
            info = add_gaps(info, console_cols);

        s = info + s;
    }

    cout << s;

    if (pb.new_line != "")
        cout << pb.new_lines << pb.new_line;

    current_console_cursor(pb.new_line_x, pb.new_line_y);

    if (!pb.cleared && pb.started && (pb.end_x > pb.new_line_x || (pb.end_x == pb.new_line_x && pb.end_y > pb.new_line_y)))
    {
        int gap_len = no_console_gaps(pb.new_line_x, pb.new_line_y, pb.end_x, pb.end_y, console_cols);
        string g;
        for (int i = 0; i < gap_len; i++)
            g += " ";
        cout << g;
    }

    pb.end_x = pb.new_line_x;
    pb.end_y = pb.new_line_y;
    pb.end_X = pb.new_line_x;
    pb.end_Y = pb.new_line_y;
    pb.current = pb.current_count;
    pb.started = 1;
    if (pb.current_count == pb.total_count && comp)
        pb.end();
}

int progress_bar_async(progress_bar_param *pb)
{
    int flfl = 0, process = 0, flag = 0;
    pb->running = 1;
    if (pb->delay < pb->delay_min)
        pb->delay = pb->delay_min;
    if (pb->delay > pb->delay_max)
        pb->delay = pb->delay_max;

    while (pb->async == 0)
    {
        if (pb->exit)
            break;
        Sleep(pb->delay);
    }

    if (!pb->exit && pb->execute)
    {
        pb->active = 1;
        while (1)
        {
            if (!pb->exit)
            {
                if (!pb->total_count)
                    continue;

                if (pb->paused)
                    continue;
                else if (pb->pause)
                {
                    pb->paused = 1;
                    pb->pause = 0;
                    continue;
                }
            }

            if (process < pb->process)
            {
                process = pb->process;
                if (!flag)
                    flag = 1;
            }
            else if (!pb->exit)
                continue;

            if (flag)
            {
                int p;
                string s, pbcs, pbrs, extra_info, new_line;
                string info;

                pbcs = pb->completed;
                pbrs = pb->remaining;
                int current_count = pb->current_count;
                extra_info = pb->extra_info;
                new_line = pb->new_line;
                int pbcc = current_count;
                if (!pb->perform && pbcc)
                    pbcc--;

                float pp = ((float)(pbcc * 100)) / pb->total_count;
                if (pp > 100)
                    pp = 100;
                p = (int)pp;
                if (pp - p > 0.5 && p != 99)
                    p++;

                if (!pb->started && !pb->save_state)
                {
                    pb->end();
                    pb->ended = 0;

                    string pbbc[] = {"▌", "█"};

                    sort(pbbc, pbbc + 2);

                    if (binary_search(pbbc, pbbc + 2, pb->completed))
                    {
                        pb->completed = "█";
                        pbcs = pb->completed;
                    }

                    pb->max_len = (pb->task + to_string(pb->total_count) + "/" + to_string(pb->total_count)).length();

                    if (pb->wide_chars)
                    {
                        pb->cp.unicode();
                        pb->mode = 1;
                    }
                    int x, y;
                    pb->current = -1;
                    current_console_cursor(x, y);
                    pb->start_X = x;
                    pb->start_Y = y;

                    if (pb->organize && x == pb->prog_start_x && pb->first_line)
                    {
                        cout << endl;
                        x++;

                        if (y)
                        {
                            cout << endl;
                            x++;
                        }
                        y = 0;
                    }
                    else if (pb->organize && x && pb->end_X)
                    {
                        if (x == pb->end_X && !y)
                        {
                            x++;
                            cout << endl;
                            y = 0;
                        }
                        else if (x == pb->end_X || y)
                        {
                            cout << endl
                                 << endl;
                            x += 2;
                            y = 0;
                        }
                    }
                    pb->start_x = x;
                    pb->start_y = y;
                    if (!pb->cursor)
                        console_cursor(0);
                }

                if (current_count <= pb->current && pb->task == "" && new_line == "" && extra_info == "" && !pb->exit && !pb->perform)
                    continue;

                float per = 100.0 / pb->blocks;
                int chars = pp / per;

                for (int k = 0; k < chars; k++)
                    s += pbcs;

                if (pb->completed == "█")
                {
                    float fx = fmod(pp, per);
                    if (fx >= per / 2)
                    {
                        chars++;
                        s += "▌";
                    }
                }

                for (int i = 0; i < pb->blocks - chars; i++)
                    s += pbrs;

                s = pb->task_lines + pb->tab + s + " ";
                if (p < 10)
                    s += "  ";
                else if (p < 100)
                    s += " ";
                s += to_string(p);

                if (pb->percentage_gap)
                    s += " ";
                s = s + "%";

                if (pb->task != "" || extra_info != "")
                {
                    if (pb->task != "" && extra_info != "")
                    {
                        if (!pb->set_len)
                            info = fill_gap(pb->task + to_string(current_count) + "/" + to_string(pb->total_count), pb->max_len) + extra_info;
                        else
                            info = fill_gap(pb->task + to_string(current_count) + "/" + to_string(pb->total_count), pb->set_len) + extra_info;
                    }
                    else if (pb->task != "")
                        info = pb->task + to_string(current_count) + "/" + to_string(pb->total_count);
                    else if (extra_info != "")
                        info = extra_info;

                    info = replace_string_all(info, "\t", pb->tab);
                    info = replace_string_all(info, "\n", "");

                    int last_info_len = pb->info_len;

                    pb->info_len = info.length();
                    s = info + s;
                }
                current_console_cursor(pb->start_x, pb->start_y, 1);

                if (pb->cleared == 1)
                {
                    erase_console(pb->start_x, pb->start_y);
                    pb->cleared = 0;
                }

                current_console_cursor(pb->start_x, pb->start_y, 1);
                cout << s;

                if (new_line != "")
                    cout << pb->new_lines << new_line;

                current_console_cursor(pb->new_line_x, pb->new_line_y);

                pb->end_x = pb->new_line_x;
                pb->end_y = pb->new_line_y;
                pb->end_X = pb->new_line_x;
                pb->end_Y = pb->new_line_y;
                pb->current = current_count;
                pb->started = 1;
                if (current_count >= pb->total_count && pb->perform)
                    break;
            }
            if ((pb->exit || pb->current_count == pb->total_count) && !flfl)
                flfl = 1;
            else if (pb->exit)
                break;
            Sleep(pb->delay);
        }
        if (!pb->save_state)
            pb->end();
    }

    pb->exit = 0;
    pb->process = 0;
    pb->running = 0;
    return 0;
}

int keypress(int k1, int k2 = 0, int k3 = 0, int k4 = 0, int *flag = NULL, int delay = 50)
{
    int k = k1;
    if (k4)
        k = k4;
    else if (k3)
        k = k3;
    else if (k2)
        k = k2;

    if ((GetAsyncKeyState(k1) & 0x8000) && (!k2 || (GetAsyncKeyState(k2) & 0x8000)) &&
        (!k3 || (GetAsyncKeyState(k3) & 0x8000)) && (!k4 || (GetAsyncKeyState(k4) & 0x8000)))
    {
        while (GetAsyncKeyState(k) != 0)
            Sleep(delay);
        return 1;
    }
    return 0;
}

struct pause_param
{
    int execute = 1;
    std::future<int> rv;
    int async = 1;
    int running = 0;
    int stop = 0;
    int exit = 0;
    int exited = 0;
    int started = 0;
    int stopped = 0;
    int delay = 50;
    int delay_max = 500;
    int delay_min = 20;
    int paused = 0;
    int resuming = 0;
    int delayed_message = 0;
    int message = 0;
    int k1 = VK_CONTROL;
    int k2 = VK_SHIFT;
    int k3 = 0;
    int k4 = 0;
    int stop_k1 = VK_CONTROL;
    int stop_k2 = VK_END;
    int once = 0;
    int current = 0;
    int resume_delay = 100;
    int messaging = 0;
    int STOP = 0;
    progress_bar_param *pb = NULL;
    string title = "";
    HWND window = GetForegroundWindow();

    pause_param(progress_bar_param *pbp, string title_ = "");

    void launch();
    void terminate();
    void relaunch();
    int resume();
};

pause_param::pause_param(progress_bar_param *pbp, string title_)
{
    pb = pbp;
    title = title_;
}

void pause_param::terminate()
{
    cout.flush();
    if (!this->execute)
        return;
    if (this->running)
    {
        this->exit = 1;
        while (this->running)
            Sleep(this->delay);
    }
}
void pause_param::relaunch()
{
    if (!this->execute)
        return;
    this->terminate();
    this->launch();
}

int pause_param::resume()
{
    if (!this->execute || (!this->paused && !this->STOP))
        return 0;
    int retval = 0;
    this->resuming = 1;
    this->once = 0;
    this->current = 0;

    if (this->pb)
    {
        if (this->pb->running)
        {
            this->pb->save_state = 1;
            this->pb->terminate();
        }
    }

    int x = 0, y = 0;

    if (!this->STOP)
    {
        current_console_cursor(x, y);

        if (this->pb)
        {
            if (x < this->pb->end_X || (x == this->pb->end_X && y < this->pb->end_Y))
            {
                x = this->pb->end_X;
                y = this->pb->end_Y;
                current_console_cursor(x, y, 1);
            }
        }

        if (y != 0)
            cout << "\n\n\t";
        else
            cout << "\n\t";

        setConsoleColor(176);
        cout << "   Paused...  ";
        while (this->messaging)
            Sleep(this->resume_delay);
    }

    this->terminate();
    if (!this->STOP)
        while (1)
        {
            if (this->window == GetForegroundWindow())
                if (keypress(this->stop_k1, this->stop_k2))
                {
                    retval = 1;
                    break;
                }

            if (this->window == GetForegroundWindow())
                if (keypress(this->k1, this->k2, this->k3, this->k4))
                    break;

            Sleep(this->resume_delay);
        }

    this->launch();
    if (this->pb)
    {
        this->pb->save_state = 0;
        if (this->pb->thread)
            this->pb->launch();
    }

    if (!this->STOP)
    {
        setConsoleColor(7);
        erase_console(x, y);
    }
    else
    {
        this->STOP = 0;
        retval = 1;
    }
    this->paused = 0;
    this->resuming = 0;
    if (this->pb)
    {
        this->pb->paused = 0;
        this->pb->pause = 0;
    }
    cout.flush();
    return retval;
}

int pause(pause_param *pp);

void pause_param::launch()
{
    if (!this->execute)
        return;
    if (!this->running)
    {
        this->rv = std::async(std::launch::async, pause, this);
        while (!this->running)
            Sleep(this->delay);
    }
}

void message_box(string m, string prog_name = "")
{
    MessageBoxA(NULL, m.c_str(), prog_name.c_str(), MB_OK);
}

void set_console_title(string s)
{
    SetConsoleTitleA(s.c_str());
}

string directory()
{
    char Buffer[MAX_PATH];
    GetCurrentDirectoryA(MAX_PATH, Buffer);
    return Buffer;
}

void md_if(string s)
{
    system(("if not exist \"" + s + "\" md \"" + s + "\"").c_str());
}

int arg_find(string str, int k = 1)
{
    string s;
    if (k == 0)
        k = 1;
    for (int i = k; i < argcg; i++)
    {
        s = argvg[i];
        if (s == str)
            return i;
    }
    return 0;
}

string arg_value(string str, string *st = NULL, int k = 1)
{
    string s;
    for (int i = k; i < argcg - 1; i++)
    {
        s = argvg[i];
        if (s == str)
        {
            s = argvg[i + 1];
            if (st)
                *st = s;
            return s;
        }
    }
    return "";
}

string get_first_line(string file)
{
    string s;
    ifstream text(file);
    if (!text)
        return "";
    getline(text, s);
    text.close();
    return s;
}

bool del_file(string s, HANDLE h = NULL)
{
    if (h)
        CloseHandle(h);
    return DeleteFileA(s.c_str());
}

void text_to_vector(string file, vector<string> &v, int column = 0, int skip_empty = 1, int skip_first = 0)
{

    if (!exists(file))
        return;

    ifstream text(file);

    string s;
    if (skip_first && !text.eof())
        getline(text, s);

    while (!text.eof())
    {
        getline(text, s);

        if (s.length() == 0 && skip_empty == 1)
            continue;
        if (column)
            v.push_back(get_column(s, column));
        else
            v.push_back(s);
    }
    text.close();

    return;
}

int binary_search_string_vec(vector<string> &arr, int start, int end, string x, int col = 0)
{
    int mid;
    if (start == end)
        return -1;
    end--;
    while (start <= end)
    {
        mid = (start + end) / 2;

        if (!col)
        {
            if (arr[mid] == x)
                return mid;
            else if (arr[mid] < x)
                start = mid + 1;
            else
                end = mid - 1;
        }
        else
        {
            if (get_column(arr[mid], col) == x)
                return mid;
            else if (get_column(arr[mid], col) < x)
                start = mid + 1;
            else
                end = mid - 1;
        }
    }
    return -1;
}

string abs_path(string s)
{
    char buffer[4096];
    GetFullPathNameA(s.c_str(), 4096, buffer, NULL);
    return buffer;
}

string time()
{
    time_t timetoday;
    time(&timetoday);
    return replace_string_all(asctime(localtime(&timetoday)), "\n", "");
}

size_t file_size(string file)
{
    fstream fs(file, fstream::in | fstream::binary);
    fs.seekg(0, ios::end);

    size_t size = fs.tellg();
    if (size == lpn)
        size = 0;
    return size;
}

fstream append_file(string file, int create_new = 0)
{
    if (!file_size(file) || create_new)
    {
        ofstream temp(file);
        temp.close();

        fstream text(file, fstream::out);
        return text;
    }

    fstream text(file, fstream::in | fstream::out);

    text.seekg(-1, fstream::end);
    char c = text.get();
    if (c != '\n' && c != '\r')
        text << "\n";
    return text;
}

inline bool network_connection()
{
    return InternetCheckConnectionA("https://one.one.one.one", FLAG_ICC_FORCE_CONNECTION, 0);
}


DWORD shell_execute(string process, string param="", string dir="", int wait=1, int show=0)
{
    if(dir=="") dir = ".";
    SHELLEXECUTEINFOA info;

    info.cbSize = sizeof(SHELLEXECUTEINFO);
    info.fMask = SEE_MASK_NOCLOSEPROCESS;
    info.hwnd = NULL;
    info.lpVerb = "open";
    info.lpFile = process.c_str();
    info.lpParameters = param.c_str();
    info.lpDirectory = dir.c_str();
    info.hInstApp = NULL;

    if(!show) info.nShow = SW_HIDE;
    else      info.nShow = SW_SHOWDEFAULT;
    
    DWORD pid = ShellExecuteExA(&info);
    if(!pid) return -1;

    if(wait && info.hProcess !=NULL)
    {
        ::WaitForSingleObject(info.hProcess, INFINITE);
        ::CloseHandle(info.hProcess);
    }

    pid = GetProcessId(info.hProcess);

    return pid;
}

struct create_process_wo__//create_process_with_output
{
    string   std_out;
    string   std_err;
    uint32_t retval;

    int create(string CmdLine, string CmdRunDir="", int window=0)
    {
        if(CmdRunDir=="") CmdRunDir = ".";
        
        this->std_out = "";
        this->std_err  = "";
        this->retval= -INT_MAX;

        int                  Success;
        SECURITY_ATTRIBUTES  security_attributes;
        HANDLE               stdout_rd = INVALID_HANDLE_VALUE;
        HANDLE               stdout_wr = INVALID_HANDLE_VALUE;
        HANDLE               stderr_rd = INVALID_HANDLE_VALUE;
        HANDLE               stderr_wr = INVALID_HANDLE_VALUE;
        PROCESS_INFORMATION  process_info;
        STARTUPINFOA         startup_info;
        thread               stdout_thread;
        thread               stderr_thread;

        security_attributes.nLength              = sizeof(SECURITY_ATTRIBUTES);
        security_attributes.bInheritHandle       = TRUE;
        security_attributes.lpSecurityDescriptor = nullptr;

        if (!CreatePipe(&stdout_rd, &stdout_wr, &security_attributes, 0) ||
                !SetHandleInformation(stdout_rd, HANDLE_FLAG_INHERIT, 0)) {
            return -1;
        }

        if (!CreatePipe(&stderr_rd, &stderr_wr, &security_attributes, 0) ||
                !SetHandleInformation(stderr_rd, HANDLE_FLAG_INHERIT, 0)) {
            if (stdout_rd != INVALID_HANDLE_VALUE) CloseHandle(stdout_rd);
            if (stdout_wr != INVALID_HANDLE_VALUE) CloseHandle(stdout_wr);
            return -2;
        }

        ZeroMemory(&process_info, sizeof(PROCESS_INFORMATION));
        ZeroMemory(&startup_info, sizeof(STARTUPINFO));

        startup_info.cb         = sizeof(STARTUPINFO);
        startup_info.hStdInput  = 0;
        startup_info.hStdOutput = stdout_wr;
        startup_info.hStdError  = stderr_wr;

        if(stdout_rd || stderr_rd)
            startup_info.dwFlags |= STARTF_USESTDHANDLES;

        char      CmdLineStr[MAX_PATH];
        strncpy(CmdLineStr, CmdLine.c_str(), MAX_PATH);
        CmdLineStr[MAX_PATH-1] = 0;

        if(!window) window = CREATE_NO_WINDOW;
        Success = CreateProcessA(
            nullptr,
            CmdLineStr,
            nullptr,
            nullptr,
            TRUE,
            window,
            nullptr,
            CmdRunDir.c_str(),
            &startup_info,
            &process_info
        );
        CloseHandle(stdout_wr);
        CloseHandle(stderr_wr);

        if(!Success) {
            CloseHandle(process_info.hProcess);
            CloseHandle(process_info.hThread);
            CloseHandle(stdout_rd);
            CloseHandle(stderr_rd);
            return -4;
        }
        else {
            CloseHandle(process_info.hThread);
        }

        if(stdout_rd) {
            stdout_thread=thread([&]() {
                DWORD  n;
                const size_t bufsize = 1000;
                char         buffer [bufsize];
                for(;;) {
                    n = 0;
                    int Success = ReadFile(
                        stdout_rd,
                        buffer,
                        (DWORD)bufsize,
                        &n,
                        nullptr
                    );
                    if(!Success || n == 0)
                        break;
                    string s(buffer, n);
                    this->std_out += s;
                }
            });
        }

        if(stderr_rd) {
            stderr_thread=thread([&]() {
                DWORD        n;
                const size_t bufsize = 1000;
                char         buffer [bufsize];
                for(;;) {
                    n = 0;
                    int Success = ReadFile(
                        stderr_rd,
                        buffer,
                        (DWORD)bufsize,
                        &n,
                        nullptr
                    );
                    if(!Success || n == 0)
                        break;
                    string s(buffer, n);
                    this->std_err += s;
                }
            });
        }

        WaitForSingleObject(process_info.hProcess,    INFINITE);
        if(!GetExitCodeProcess(process_info.hProcess, (DWORD*) &(this->retval)))
            this->retval = -1;

        CloseHandle(process_info.hProcess);

        if(stdout_thread.joinable())
            stdout_thread.join();

        if(stderr_thread.joinable())
            stderr_thread.join();

        CloseHandle(stdout_rd);
        CloseHandle(stderr_rd);

        return 0;
    }

};



/////////////////////////////////////////////////////////////////////////////////////


void sys(string s)
{
    system(s.c_str());
}


int dir_content(string s, vector<string> *v=NULL, int dir=0, int subf=1, int all=0)
{
    if(!exists(s) || !exists(s+"\\")) return 0;

    string cmd = "dir /b ";
    if(subf)     cmd += "/s ";
    
    if(all)      cmd += "/a ";
    else if(dir) cmd += "/a:d ";
    else         cmd += "/a-d ";

    cmd = cmd+"\""+s+"\"";

    s = pipe_open(cmd);

    if(!v) return str_find_count(s,"\n");

    size_t len=s.length();
    if(len && (s[len-1]=='\n')) s.erase(len-1,1);
    stringstream ss = stringstream(s);
    while(!ss.eof())
    {
        getline(ss,s,'\n');
        v->push_back(s);
    }
    return v->size();
}

inline int remove_dir(string s)
{
    return RemoveDirectoryA(s.c_str());
}

inline void delete_if(string s)
{
    system(("if exist \""s"\" del \""+s+"\"").c_str());
}

int writable_dir(string dir=directory())
{
    if(lower_case_converter(dir)=="c:\\windows\\system32" || !exists(dir)) return 0;
    if(dir[dir.length()-1]!='\\') dir += "\\";

    string tf = name_assigner(dir+"temp__");
    write_to_text(tf,"1",0);
    if(!exists(tf)) return 0;
    else delete_if(tf);
    return 1;
}

string find_in_text(string file, string str, int col=0, int exact=0)
{
    string s;
    ifstream text(file);
    if(!text)   return "";
    while(!text.eof())
    {
        getline(text,s);
        if(s.length()==0)   continue;

        if(col==0)
        {
            if(!exact) if(s.find(str)!=string::npos) return s;
            else       if(s==str)                    return s;
        }
        else
        {
            if(!exact) if(get_column(s,col).find(str)!=string::npos) return s;
            else       if(get_column(s,col)==str)                    return s;
        }
    }
    return "";
}


struct aria2_progress_bar_param
{
    int execute=1;
    std::future<int> rv;
    int async=1;
    int stop=0;
    int exit=0;
    int running=0;
    int progress_done=0;
    int started=0;
    int stopped=0;
    int precision=0;
    int set_len=37;
    //string separator = "\tSpeed : ";
    string separator = "   ";
    string task;
    int clear=0;
    int start_x=0;
    int percentage_gap=1;
    int resize=1;
    int display=1;
    int cursor=0;
    int once=0;
    int console_size=95;
    int wide_chars=1;
    int bell=0;
    int mode=0;
    int text_open=0;
    int delay=500;
    int delay_max=500;
    int delay_min=200;
    int progress_delay=1000;
    int organize=1;
    int start_X=0;
    int start_Y=0;
    int end_X=0;
    int end_Y=0;
    int first_line=1;

    progress_bar_param *pb=NULL;
    codepage__ cp;
    string completed="█";
    string remaining="░";

    string tab="        ";
    string start_lines;
    string task_lines="\n\n";
    string new_lines="\n\n";
    string end_lines="\n\n";

    string progress_file;

    aria2_progress_bar_param(progress_bar_param *pbp, string file);

    void launch();
    void _stop();
    void terminate();
    void relaunch();
};

aria2_progress_bar_param::aria2_progress_bar_param(progress_bar_param *pbp, string file)
{
    pb = pbp;
    progress_file = file;
}

void aria2_progress_bar_param::_stop()
{
    if(!this->execute || !this->pb)  return ;
    if(this->started)
    {
        this->stop = 1;
        while(this->started) Sleep(this->delay);
    }
}

void aria2_progress_bar_param::terminate()
{
    if(!this->execute || !this->pb)  return ;
    if(this->running)
    {
        this->exit = 1;
        while(this->running) Sleep(this->delay);
    }
}
void aria2_progress_bar_param::relaunch()
{
    if(!this->execute || !this->pb)  return ;
    this->terminate();
    this->launch();
}

void aria2_progress_bar_param::launch()
{
    if(!this->execute || !this->pb)  return ;
    delete_if(this->progress_file);
    if(!this->running)
    {
        this->rv = std::async(std::launch::async, aria2_progress_bar_async,this);
        while(!this->running) Sleep(this->delay);
    }
}


int aria2_progress_bar_async(aria2_progress_bar_param *apbps)
{
    if(!apbps->execute || !apbps->pb)  return 0;
    apbps->running=1;
    string file = apbps->progress_file;

    if(apbps->progress_delay>2500)          apbps->progress_delay = 2500;
    else if(apbps->progress_delay<500)      apbps->progress_delay = 500;

    if(apbps->delay<apbps->delay_min)       apbps->delay   = apbps->delay_min;
    else if(apbps->delay>apbps->delay_max)  apbps->delay   = apbps->delay_max;

    apbps->pb->set_len = apbps->set_len;

starting :

    while(apbps->async==0)
    {
        if(apbps->exit) break;
        else continue;
        Sleep(apbps->delay);
    }
    apbps->started=1;

if(!apbps->exit)
{
    while(!exists(file))
    {
        Sleep(apbps->progress_delay);
        if(apbps->exit) break;
    }

    string s, speed_str;
    stringstream ss;
    int i=0;
    float speed;

    string progress = text_to_string(file);

    while(!str_find(progress,"Downloading "))
    {
        progress = text_to_string(file);

        Sleep(apbps->progress_delay);
        if(apbps->exit) break;
    }

    i = str_find_index(progress,"Downloading ") + 12;

    int str_len=progress.length();

    while(progress[i]!=' ' && i<str_len)
    {
        s.push_back(progress[i]);
        i++;
    }
    if(is_int(s))  apbps->pb->total_count = stoi(s);

    size_t f;
    int ff=0, gg=0;
    apbps->stop = 0;

    int hf=0;
    int flfl=0;
    int kk=0;
    //if(apbps->pb->total_count && str_find_count(progress," Download complete: ")<=apbps->pb->total_count && !apbps->exit)
    //while(!str_find(progress,"aria2 will resume download if the transfer is restarted."))
    while(1)
    {
        progress = text_to_string(file);

        //cout_c("cc",pb.current_count,"tc",pb.total_count);
        //cout_l(str_find_count(dts.update," Download complete: "));
        //apbps->pb->current_count = apbps->pb->current_count + str_find_count(dts.update," Download complete: ");
        apbps->pb->current_count = str_find_count(progress," Download complete: ");

        f = progress.rfind("DL:");
        if(f!=string::npos)
        {
            i = f+3;
            speed_str = "";
            int str_len = progress.length();
            while(1)
            {
                if(progress[i]==']' || progress[i]==' ' || i>=str_len)    break;
                speed_str.push_back(progress[i]);
                i++;
                //if(apbps->exit) break;
            }
            ss = stringstream(speed_str);
            ss>>speed;

            while(speed_str[0]>='0' && speed_str[0]<='9' || speed_str[0]=='.')
            speed_str.erase(0,1);
            if(speed_str[1]=='i') speed_str.erase(1,1);
            speed = speed * 1.024;

            s = to_string(speed);
            f = s.find(".");
            if(f!=string::npos)
            s.erase(f+3,s.length()-3);
            speed_str = s + " " + speed_str;
            gg=1;
        }
        else if(!gg)    speed_str = "0.00 B";
        //if(str_find(speed_str," ps")) speed_str
        apbps->pb->extra_info = apbps->separator + speed_str + "ps";

        if(flfl || apbps->pb->current_count>=apbps->pb->total_count)
             progress_bar(*(apbps->pb),1);
        else progress_bar(*(apbps->pb));

        hf=1;

        if(apbps->pb->current_count>=apbps->pb->total_count) break;
        if(apbps->exit && flfl) break;
        else if(apbps->exit)    flfl=1;

        Sleep(apbps->progress_delay);
    }

    apbps->pb->end();
}
    apbps->started = 0;
    apbps->exit = 0;
    apbps->running=0;
    return 0;
}



void exit_now(int argc, string s="", progress_bar_param *pb=NULL,
pause_param *pp=NULL, aria2_progress_bar_param *apbps=NULL, int exit_code=0)
{
    if(pb) pb->terminate();
    if(pp) pp->terminate();
    if(apbps) apbps->terminate();

    int x, y;
    current_console_cursor(x,y);
    if(y) cout<<endl;
    if(s!="")  cout<<"\n"<<s<<endl;
    console_cursor(1);

    if(argc==1)
    {
        cout<<"\n\tPress any key to EXIT...";
        getch();
        cout<<endl;
    }
    exit(exit_code);
}


struct exit__
{
    int argc=1;
    string msg="";
    string arg_msg="";
    progress_bar_param *pb=NULL;
    pause_param *pp=NULL;
    aria2_progress_bar_param *apbps=NULL;
    int exit_code=0;

    exit__(int argc_, string msg_="", string arg_msg_="",
    progress_bar_param *pb_=NULL, pause_param *pp_=NULL, aria2_progress_bar_param *apbps_=NULL)
    {
        argc  = argc_;
        msg   = msg_;
        pb    = pb_;
        pp    = pp_;
        apbps = apbps_;
        arg_msg=arg_msg_;
    }

    void exit_now(string msg_="", string arg_msg_="", int exit_code=0)
    {
        if(msg_!="")    this->msg = msg_;
        if(arg_msg_!="")this->arg_msg = arg_msg_;

        if(this->pp)    this->pp->terminate();
        if(this->apbps) this->apbps->terminate();

        if(this->pb)
        {
            this->pb->terminate();
            if(this->pb->organize)
            {
                int x, y;
                current_console_cursor(x,y);
                if(!this->pb->clear && x==this->pb->end_X && y==this->pb->end_Y
                    && this->pb->end_lines.find("\n\n")!=string::npos)
                console_pointer("up");            
            }
        }

        int x, y;
        current_console_cursor(x,y);
        if(y) cout<<endl;
        if(this->msg!="")  cout<<"\n"<<this->msg<<endl;
        console_cursor(1);

        if(this->argc==1)
        {
            if(this->arg_msg!="") cout<<endl<<this->arg_msg<<endl;
            cout<<"\n\tPress any key to EXIT...";
            getch();
            cout<<endl;
        }
        exit(exit_code);
    }
};

string * text_to_string_array(string file, int * n, int column=0, int skip_empty=1, int skip_first=0)
{
    ifstream text(file);
    int lines = lines_of_text(file,skip_empty);
    if(lines<=0)
    {
        *n = 0;
        text.close();
        return NULL;
    }

    string s;
    int i=0;

    if(skip_first)
    {
        getline(text,s);
        if(s!="")   lines--;
    }

    string * arr = new string[lines];

    while(!text.eof())
    {
        getline(text,s);
        if(s.length()==0 && skip_empty==1)   continue;
        if(column)     arr[i] = get_column(s,column);
        else            arr[i] = s;
        i++;
    }
    text.close();
    *n = lines;
    return arr;
}

void psv_sorter(string * arr, int n, int col=0, int reverse=0)
{
    if (n < 2)
        return;

    sort(arr, arr+n, [col, reverse](string &s1, string &s2) {
        if(!reverse)
            return get_column(s1,col) < get_column(s2,col);

        return get_column(s1,col) > get_column(s2,col);
    });
}

int binary_search_string(string arr[], int start, int end, string x, int col=0)
{
    int mid;
    if(start==end) return -1; 
    end--;
    while(start<=end)
    {
        mid = (start+end)/2;

        if(!col)
        {
            if(arr[mid]==x)
                return mid;
            else if(arr[mid]<x)
                start = mid + 1;
            else
                end = mid - 1;
        }
        else
        {
            if(get_column(arr[mid],col)==x)
                return mid;
            else if(get_column(arr[mid],col)<x)
                start = mid + 1;
            else
                end = mid - 1;
        }
    }
    return -1;
}
int arg_as_int(string str, int *n=NULL, int k=1)
{
    string s = arg_value(str,NULL,k);
    if(is_int(s))
    {
        if(n) *n = stoi(s);
        return 1;
    }
    return 0;
}

inline int hex2i(string s)
{
    return strtol(s.c_str(), NULL, 16);
}

void ss_le(string file, vector<string> &v)
{
    if(!exists(file))   return ;

    string s;

    ifstream text(file);
    stringstream ss;
    ss << text.rdbuf();
    string body = ss.str();
    text.close();

    size_t last = body.rfind("*************************");
    if(last==lpn) return ;
    size_t f = body.find("/song/");
    while(f!=string::npos && f<last)
    {
        // s = "*************************";

        while(body[f]!='"')
        {
            s.push_back(body[f]);
            f++;
        }
        v.push_back(s);
        f = body.find("*************************", f+1);
    }
    return ;
}

template <typename T>
void vector_to_text(string file, vector<T> &v, int append=1)
{
    fstream fs;

    if(!append) del_file(file);
    fs = append_file(file);
    
    size_t vs = v.size();
    for(size_t i=0; i<vs; i++)
    {
        if(i) fs<<endl;

        fs<<v[i];
    }
    fs.close();
}


void msg_pr(string s, int new_line=1)
{
    if(s=="") return ;

    int x, y;

    current_console_cursor(x,y);
    if(y) cout<<endl;
    if(s=="|")
    {
        cout<<endl;
        return;
    }

    cout<<"\n\t"<<s;
    if(new_line)    cout<<endl; 
}


void text_merger(string * files, int n, vector<string> &v, int flag_skip_empty=1, int flag_skip_first=0)
{
    string s;

    for(int i=0; i<n; i++)
    {
        if(!exists(files[i]))
        {
            cout<<"\ntext_merger - "<<files[i]<<" DOESN'T EXIST.\n";
            continue;
        }

        ifstream text(files[i]);
        while(!text.eof())
        {
            getline(text,s);
            if(flag_skip_first) getline(text,s);
            if(s.length()==0 && flag_skip_empty)    continue;

            v.push_back(s);
        }
        text.close();
    }

    return ;
}

string * vector_2_string_array(vector<string> v, int &n)
{
    int size = v.size();
    if(!size)   return NULL;
    n = size;
    string * arr = new string[size];
    for(int i=0; i<size; i++)
    arr[i] = v[i];
    return arr;
}

int ss_json2tags(string file, vector<string> &v, vector<string> &l, int replace_=1, string playlist="")
{
    return INT_MIN;
}

void ss_ale(string file, vector<string> &v)
{
    if(!exists(file))   return ;

    string s;
    string t= "********";
    int len = t.length();
    ifstream text(file);

    getline(text,s);
    while(!str_find(s,t))
    getline(text,s);

    while(1)
    {
        if(!str_find(s,t))  break;

        s.erase(0,s.find("https://"));
        s.erase(s.rfind("\""),s.length()-s.rfind("\""));
        v.push_back(s);
        getline(text,s);
    }
    text.close();
    return ;
}

inline string i2s(int num)
{   
    return to_string(num);
}

string ss_lyrics(string file)
{
    if(!exists(file))   return "";

    Json::Value r;
    string s, errors;
    s = text_to_string(file);
    s = replace_string_all(s,"|","");
    bool b = str2json(s,r,&errors);
    if(!b || !r.isMember("lyrics")) return "";
    string lyrics = r["lyrics"].asString();
    return lyrics;
}
int lines_of_text_0(string s, int f=1)
{
    ifstream in(s);
    if(!in)     return 0;
    int lines=0;

    while(!in.eof())
    {
        getline(in,s);
        if(f==1 && s.length()==0)   continue;
        lines++;
    }
    return lines;
}


void psv_dlr(vector<string> &v, int col=1)
{
    if(v.size()<2)  return ;
    string s;

    map<string,string> m;
    map<string,string>::iterator itr;

    vector<string>  empty_col_v;

    vector<string>::iterator vi;

    int i=0;
    for(vi=v.begin(); vi!=v.end(); ++vi)
    {
        s = v[i];
        if(s.length()==0)   continue;

        if(get_column(s,col)=="")
        {
            empty_col_v.push_back(s);
        }
        else  m.insert(pair<string, string>(get_column(s,col), s));
        i++;
    }

    v.clear();
    i=0;
    for(vector<string>::iterator itr=empty_col_v.begin(); itr!=empty_col_v.end(); ++itr)
    {
        v.push_back(empty_col_v[i]);
        i++;
    }
    empty_col_v.clear();

    for (itr = m.begin(); itr != m.end(); ++itr)
    v.push_back(itr->second);

    return ;
}


size_t dir_size(string path)
{
    WIN32_FIND_DATAA data;
    string file = path + "\\*.*";
    size_t size = 0;
    HANDLE h = FindFirstFileA(file.c_str(),&data);
    if(h != INVALID_HANDLE_VALUE)
    {
        do {
            if( (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
            {
                if( strcmp(data.cFileName,".") != 0 &&strcmp(data.cFileName,"..") != 0)
                {
                    file = path + "\\" + data.cFileName;
                    size += dir_size(file);
                }
            }
            else
            {
                LARGE_INTEGER sz;
                sz.LowPart = data.nFileSizeLow;
                sz.HighPart = data.nFileSizeHigh;
                size += sz.QuadPart;
            }
        } while( FindNextFileA(h,&data) != 0);
        FindClose(h);

    }
    return size;
}

string ss_bte(string file) {
    return file;
}

string ss_btr(string * arr, int n, string tag)
{
    string id = erase_rfind(get_column(tag,10),"/");
    if(id=="")  return tag;

    int i = binary_search_string(arr,0,n,id,7);
    if(i>=0)
    {
        string bt = arr[i];
        tag = write_column(tag,12,get_column(bt,1));
        tag = write_column(tag,10,get_column(bt,2));
        tag = write_column(tag, 2,get_column(bt,4));
        tag = write_column(tag,11,get_column(bt,5));
        tag = write_column(tag, 9,get_column(bt,6));
    }
    return tag;
}

struct tag_fields_sn__
{
    string arr[4] = { "********", "********", "********", "********" } ;   
    int n = sizeof(arr)/sizeof(arr[0]);
    int get_sn(string s)
    {
        if(s=="********" || s=="********") return INT_MIN;
        for(int i=0; i<n; i++)
        if(s==this->arr[i]) return i+1;
        
        return 0;
    }
};

string remove_tag_words(string s, string w)
{
    if(!str_find(s,w) && w!="--all")  return s;

    if(w=="from" || w=="--all")
    {
        int i, j, k=0;
        if(str_find(s,"(From"))
        {
            i = str_find_index(s,"(From");
            j = str_find_index(s,")",i);
            k=1;
        }
        else if(str_find(s,"(from"))
        {
            i = str_find_index(s,"(from");
            j = str_find_index(s,")",i);
            k=1;
        }
        if(k)
        {
            s.erase(j-1,1);

            if(s[i+5]==' ')  s.erase(i+1,6);
            else             s.erase(i+1,5);
        }
    }

    if(w=="OST" || w=="--all")  s = erase_rfind_found_to_end(s," (OST)");
    if(w==" - Single" || w=="--all")  s = erase_rfind_found_to_end(s," - Single");
    if(w=="''" || w=="--all")   s = replace_string_all(s,"''","\"");
    if(w=="Original Motion Picture Soundtrack" || w=="--all")
    s = erase_rfind_found_to_end(s," (Original Motion Picture Soundtrack)");

    return s;
}

string kid3_tag(string file, string s, string album_json_file, string kid3_cli, int other, int remove_words_=1, string curr_dir=directory())
{
    if(!exists(file) || kid3_cli=="")   return "";
    string sq, rt;
    sq.push_back('"');

    int pipes=INT_MAX;
    if(!other) pipes++;

    for(int i=0; i<INT_MAX; i++)
    rt += "|";

    string album = get_column(s,INT_MAX), title = get_column(s,INT_MAX);
    string albumartist = get_column(s,INT_MAX);

    if(remove_words_)
    {
        album = remove_tag_words(album,"--all");
        title = remove_tag_words(title,"--all");
    }

    string genre = get_column(s,INT_MAX);
    if(genre!="")  genre[0] = (char)((int)genre[0]-32);

    albumartist = replace_string_all(albumartist," -", "–");
    albumartist = replace_string_all(albumartist," - ", "–");
    albumartist = replace_string_all(albumartist,"- ", "–");

    string composer;

    rt = write_column(rt,INT_MAX,album);
    rt = write_column(rt,INT_MAX,albumartist);
    rt = write_column(rt,INT_MAX,get_column(s,INT_MAX));
    rt = write_column(rt,INT_MAX,composer);
    rt = write_column(rt,INT_MAX,get_column(s,INT_MAX));
    rt = write_column(rt,INT_MAX,genre);
    rt = write_column(rt,INT_MAX,get_column(s,INT_MAX));
    rt = write_column(rt,INT_MAX,get_column(s,INT_MAX));
    rt = write_column(rt,INT_MAX,get_column(s,INT_MAX));
    rt = write_column(rt,INT_MAX,get_column(s,INT_MAX));
    rt = write_column(rt,INT_MAX,get_column(s,INT_MAX));
    rt = write_column(rt,INT_MAX,get_column(s,INT_MAX));
    rt = write_column(rt,INT_MAX,title);
    rt = write_column(rt,INT_MAX,get_column(s,INT_MAX));
    rt = write_column(rt,INT_MAX,get_column(s,INT_MAX));
    rt = write_column(rt,INT_MAX,get_column(s,INT_MAX));
    
    rt = write_column(rt,INT_MAX,get_column(s,INT_MAX));
    rt = write_column(rt,INT_MAX,get_column(s,INT_MAX));
    rt = write_column(rt,INT_MAX,get_column(s,INT_MAX));
    rt = write_column(rt,INT_MAX,get_column(s,INT_MAX));
    rt = write_column(rt,INT_MAX,get_column(s,INT_MAX));

    album = replace_string_all(album,"'","\\'");
    album = replace_string_all(album,sq,"\\"+sq);

    albumartist = replace_string_all(albumartist,"'","\\'");
    albumartist = replace_string_all(albumartist,sq,"\\"+sq);

    title = replace_string_all(title,"'","\\'");
    title = replace_string_all(title,sq,"\\"+sq);

    genre = replace_string_all(genre,"'","\\'");
    genre = replace_string_all(genre,sq,"\\"+sq);

    composer = replace_string_all(composer,"'","\\'");
    composer = replace_string_all(composer,sq,"\\"+sq);


    s = replace_string_all(s,"'","\\'");
    s = replace_string_all(s,sq,"\\"+sq);

    string tag;
    tag = "********";


    string cover = erase_rfind(get_column(s,INT_MAX),"/");
    
    if(exists("sources\\covers\\"+cover)) 
    {
        cover = curr_dir+"sources\\covers\\"+cover;
        cover = replace_string_all(cover,"'","\\'");
        tag = tag + " -c "+sq+"set picture:'"+cover+"' front"+sq;
    }

    string lyrics = curr_dir+"sources\\lyrics\\"+erase_rfind(get_column(s,INT_MAX),"/")+".txt";
    lyrics = replace_string_all(lyrics,"'","\\'");
    if(exists(lyrics))
    tag = tag + " -c "+sq+"set lyrics:'"+lyrics+"' ''"+sq;


    tag = tag + " "+sq+file+sq;
    sys(tag);
    return rt;
}

string change_ex(string file, string ext)
{
    size_t g, f = file.rfind(".");

    if(f!=string::npos)
    {
        g = file.rfind("\\");
        if(g==string::npos) g=0;

        if(g<f)
        {
            file.erase(f+1,file.length()-f-1);
            file += ext;
        }
    }

    return file;
}

int string_arg_find(string s, string w)
{
    if(s=="" || w=="")  return 0;
    s = replace_string_all(s,"  ", " ",1);

    stringstream ss = stringstream(s);
    while(!ss.eof())
    {
        getline(ss,s,' ');
        if(s==w)    return 1;
    }
    return 0;
}

string tag_extractor(string file, string kid3_cli="C:\\Tools\\kid3\\kid3-cli.exe")
{
    if(!exists(file) || !exists(kid3_cli))   return "";

    string s, tag, t, temp, lyrics, sl;
    int f;
    string str      =  pipe_open(kid3_cli+" -c \"get all 123\" "+"\""+file+"\"");
    if(str_find(str,"\n  Lyrics                  "))    lyrics  =  pipe_open(kid3_cli+" -c \"get lyrics\" "+"\""+file+"\"");
    if(str_find(str,"\n  Synchronized Lyrics     "))    sl      =  pipe_open(kid3_cli+" -c \"get 'Synchronized Lyrics'\" "+"\""+file+"\"");

    if(lyrics.length())
    {
        t = "\n  Lyrics                  ";
        f = str_find_index(str,t);
        str.erase(f+3,t.length()-1+lyrics.length());
    }

    if(sl.length())
    {
        t = "\n  Synchronized Lyrics     ";
        f = str_find_index(str,t);
        str.erase(f+3,t.length()-1+sl.length());
    }

    str = replace_string_all(str,"|","--pipe");

    f = str_find_index(str,"\nTag 2: ");
    if(f==-1)   return "";

    f = str_find_index(str,"\n",f+1);
    str.erase(0,f+1);

    int lines = str_find_count(str,"\n");
    str.erase(str.length()-1,1);

    string * tag_arr = new string[lines];
    stringstream ss = stringstream(str);
    int len;
    char c;
    int i=0;
    while(!ss.eof())
    {
        getline(ss,s,'\n');
        len = s.length();
        if(!len)    continue;

        f = str_find_index(s,"  ",2);
        if(f==-1)
        {
            cout<<"\ntag_extractor - tag field of string \""<<s<<"\" is too long.\n";
            return "";
        }

        t = "";

        for(int i=2; i<f; i++)
        {
            if(s[i]!=' ')
            {
                c = s[i];
                if(c<='Z' && c>='A')  c = (char)((int)c+32);
                t.push_back(c);
            }
        }
        if(t=="date")                           t = "year";
        else if(t=="tracknumber")               t = "track";
        else if(t=="originalalbum")             t = "origalbum";
        else if(t=="originalartist")            t = "origartist";
        else if(t=="originaldate")              t = "origyear";
        else if(t=="originalfilename")          t = "origfilename";
        else if(t=="originalartist")            t = "origartist";
        else if(t=="work")                      t = "contentgroup";
        else if(t=="commercialurl")             t = "wwwcommercialinfo";
        else if(t=="copyrighturl")              t = "wwwcopyright";
        else if(t=="officialradiostation")      t = "wwwradiopage";
        else if(t=="officialpublisher")         t = "wwwpublisher";
        else if(t=="payment")                   t = "wwwpayment";
        else if(t=="©con")                      t = "conductor";
        else if(t=="website")                   t = "wwwartist";
        else if(t=="encoded-by")                t = "encodedby";

        t += "|";

        while(s[f]==' ' && f<len) f++;
        while(f<len)
        {
            t.push_back(s[f]);
            f++;
        }

        tag_arr[i] = t;
        i++;
    }

    psv_sorter(tag_arr,lines,1);
    string arr[] = {"fileowner", "album", "albumartist", "artist", "barcode",
    "compilation", "composer", "conductor", "contentgroup", "copyright",
    "discnumber", "genre", "initialkey", "isrc", "language", "length",
    "origalbum", "origartist", "origfilename", "origyear", "publisher",
    "source", "sourceid", "title", "track", "url", "playlist",
    "wwwaudiofile", "wwwaudiosource", "wwwcommercialinfo", "wwwcopyright",
    "wwwpublisher", "wwwradiopage", "year", "encodedby", "sourceurl"};

    int n = sizeof(arr)/sizeof(string);

    int count = 0;
    for(int i=0; i<n; i++)
    {
        if(i>0) tag += "|";
        f = binary_search_string(tag_arr,0,lines,arr[i],1);

        if(f!=-1)  tag += get_column(tag_arr[f],2);
    }
    return tag;
}
string * tag_extractor_array(string file, int &n, string kid3_cli="C:\\Tools\\kid3\\kid3-cli.exe")
{
    if(!exists(file) || !exists(kid3_cli))   return NULL;

    string s, tag, t, temp, lyrics, sl;
    int f;
    string str      =  pipe_open(kid3_cli+" -c \"get all 123\" "+"\""+file+"\"");
    if(str_find(str,"\n  Lyrics                  "))    lyrics  =  pipe_open(kid3_cli+" -c \"get lyrics\" "+"\""+file+"\"");
    if(str_find(str,"\n  Synchronized Lyrics     "))    sl      =  pipe_open(kid3_cli+" -c \"get 'Synchronized Lyrics'\" "+"\""+file+"\"");

    if(lyrics.length())
    {
        t = "\n  Lyrics                  ";
        f = str_find_index(str,t);
        str.erase(f+3,t.length()-1+lyrics.length());
    }
    if(sl.length())
    {
        t = "\n  Synchronized Lyrics     ";
        f = str_find_index(str,t);
        str.erase(f+3,t.length()-1+sl.length());
    }

    str = replace_string_all(str,"|","--pipe");

    f = str_find_index(str,"\nTag 2: ");
    if(f==-1)   return NULL;

    f = str_find_index(str,"\n",f+1);
    str.erase(0,f+1);
    
    int lines = str_find_count(str,"\n");
    str.erase(str.length()-1,1);

    string * tag_arr = new string[lines];
    stringstream ss = stringstream(str);
    int len;
    char c;
    int i=0;
    while(!ss.eof())
    {
        getline(ss,s,'\n');
        len = s.length();
        if(!len)    continue;

        f = str_find_index(s,"  ",2);

        if(f==-1)
        {
            cout<<"\ntag_extractor - tag field of string \""<<s<<"\" is too long.\n";
            return NULL;
        }

        t = "";

        for(int i=2; i<f; i++)
        {
            if(s[i]!=' ')
            {
                c = s[i];
                if(c<='Z' && c>='A')  c = (char)((int)c+32);
                t.push_back(c);
            }
        }
        if(t=="date")                           t = "year";
        else if(t=="tracknumber")               t = "track";
        else if(t=="originalalbum")             t = "origalbum";
        else if(t=="originalartist")            t = "origartist";
        else if(t=="originaldate")              t = "origyear";
        else if(t=="originalfilename")          t = "origfilename";
        else if(t=="originalartist")            t = "origartist";
        else if(t=="work")                      t = "contentgroup";
        else if(t=="commercialurl")             t = "wwwcommercialinfo";
        else if(t=="copyrighturl")              t = "wwwcopyright";
        else if(t=="officialradiostation")      t = "wwwradiopage";
        else if(t=="officialpublisher")         t = "wwwpublisher";
        else if(t=="payment")                   t = "wwwpayment";
        else if(t=="©con")                      t = "conductor";
        else if(t=="website")                   t = "wwwartist";
        else if(t=="encoded-by")                t = "encodedby";

        t += "|";

        while(s[f]==' ' && f<len) f++;
        while(f<len)
        {
            t.push_back(s[f]);
            f++;
        }

        tag_arr[i] = t;
        i++;
    }

    psv_sorter(tag_arr,lines,1);

    n = lines;

    return tag_arr;
}

string renamer(string format, string dir, int other_files=0, progress_bar_param * pb=NULL, pause_param *pp=NULL, string log_dir="", string kid3_cli="C:\\Tools\\kid3\\kid3-cli.exe" , string extension="m4a mp3 aac flac")
{
    if(format=="" || dir=="" || !exists(kid3_cli) || !dir_size(dir))   return "";

    string s, str, t, tag, value, file;

    int pbf=0, ppf=0;
    if(pb)
    {
        if(pb->execute)  pbf = 1;
    }
    if(pp)
    {
        if(pp->execute)  ppf = 1;
    }

    if(dir[dir.length()-1]!='\\')   dir += "\\";   

    format = replace_string_all(format,"|","--pipe");
    format = validate_path(format);
    format = replace_string_all(format,"--pipe","|");

    string format_ = format;
    
    int xf=0;
    int f = str_find_index(format,"|extension|=");
    if(f!=-1)
    {
        f += 12;
        if(str_find_index(format,"|",f)) xf=1;
    }

    if(str_find_count(format,"|")%2==1 || str_find_count(format,"|extension|=")>1 || xf)
    {
        msg_pr("Directory Renaming Format is invalid.\n");
        return "";
    }

    string file_str = pipe_open("dir /s /b /a-d \""+dir+"\"");
    stringstream ss = stringstream(file_str);

    vector<string> v;
    if(pbf)    pb->current_count=0;
    if(pbf)    pb->total_count=0;
    string temp_str;

    while(!ss.eof())
    {
        getline(ss,s);
        if(!s.length())    continue;
        t = ext_ex(s);
        if(t[0]=='.')   t.erase(0,1);

        if(string_arg_find(extension,t))
        {
            if(!other_files)
            {
                temp_str = tag_extractor(s,kid3_cli);
                temp_str = get_column(temp_str,29);
                if(str_find(temp_str,"********"))
                {
                    if(pbf) pb->total_count++;
                    v.push_back(s);
                }
            }
            else if(other_files) 
            {
                if(pbf) pb->total_count++;
                v.push_back(s);
            }
        }
    }

    int i=0, vec_size = v.size(), current_index=0, pipe;

    string ext_;

    format = replace_string_all(format,"|filename|", "/");
    format = replace_string_all(format,"|directory|", ">");

    f = str_find_index(format,"|extension|=");

    if(f!=-1)
    {
        format.erase(f,12);
        int len=format.length();

        for(int i=f; i<len; i++)
        ext_.push_back(format[i]);

        format.erase(f,len-f);
    }

    //format = replace_string_all(format,"|extension|=", "<");
    


    pipe = str_find(format,"|");
    vector <string> field;

    f = str_find_index(format,"|");
    while(f!=-1)
    {
        string t;
        i=f+1;

        while(format[i]!='|')
        {
            t.push_back(format[i]);
            i++;
        }

        if(t.length()) 
        {
            field.push_back(t);
            format.replace(f,t.length()+2,"*");
        }
        else    format.erase(f,2);
        f = str_find_index(format,"|");
    }

    format_ = format;
    string nn, nn_temp, on_trimmed, nn_trimmed;

    string * tag_arr;
    int lines_tag=0, star=0;
    int field_size = field.size();
    

    string curr_dir = directory();
    SetCurrentDirectoryA(dir.c_str());
    string dir_full_path = directory();
    SetCurrentDirectoryA(curr_dir.c_str());
    int dir_len = dir_full_path.length();
    
    if(pbf) if(pb->thread) pb->relaunch();
    if(ppf) pp->relaunch();

    int ren_dir_fail=0;

    if(log_dir!="") md_if(log_dir);
    string log_file;
    if(log_dir=="") log_file = "dir-ren_"+validate_path(time())+".log";
    else
    {
        int len = log_dir.length();
        if(log_dir[len-1]!='\\') log_dir += "\\";
        log_file = log_dir+replace_string_all(time(),":",",")+".log";
    }
    bool b=0;
    while(current_index < vec_size)
    {
        if(pbf) pb->current_count++;

        file = v[current_index];
        format = format_;

        if(pipe)
        {
            tag_arr = tag_extractor_array(file,lines_tag,kid3_cli);

            for(int i=0; i<field_size; i++)
            {
                star = str_find_index(format,"*");
                t = field[i];

                f = binary_search_string(tag_arr,0,lines_tag,t,1);
                s = "";
                if(f!=-1) s = validate_path(get_column(tag_arr[f],2));

                format.replace(star,1,s);
            }
            if(lines_tag) delete [] tag_arr;
        }

        format = replace_string_all(format,"/",del_ex(erase_rfind(file,"\\")));
        format = replace_string_all(format,">",erase_rfind_found_to_end(file,"\\"));
        
        format = validate_path(format);

        if(ext_!="") nn = format + "."+ext_;
        else         nn = format+ext_ex(file);
        
        nn_temp = nn;
        if(file!=nn) nn_temp = name_assigner(nn_temp);

        if(pbf)
        {
            on_trimmed = file;
            nn_trimmed = nn_temp;

            on_trimmed.erase(0,dir_len+1);
            nn_trimmed.erase(0,dir_len+1);

            pb->new_line = "\t" + on_trimmed + pb->transition + nn_trimmed;
            pb->update();
        }

        if(ppf) pp->resume();

        md_if(erase_rfind_found_to_end(format,"\\"));

        if((ext_dn(file)).length()>220 || file.length()>259 || (ext_dn(nn_temp)).length()>220 || nn_temp.length()>259)
        {
            if(!ren_dir_fail)
            {
                Sleep(1000);                
                ren_dir_fail=1;
            }
            string err;
            if(file.length()>259 && nn_temp.length()>259)   err = "Source and destination are too long";
            else if(file.length()>259)                      err = "Source too long";
            else                                            err = "Destination too long";

            write_to_text(log_file,err+"|"+file+"|"+nn_temp);
            current_index ++;
            continue;
        }

        b=0;

        if(file!=nn)  b = MoveFileExA(file.c_str(),(nn_temp).c_str(),1|2);
        else
        {
            write_to_text(log_file,"Source and destination are same|"+file+"|"+nn_temp);
            current_index++;
            continue;
        }

        if(b) write_to_text(log_file,"Renamed|"+file+"|"+nn_temp);
        else  write_to_text(log_file,"Unknown error|"+file+"|"+nn_temp);
        current_index++;
    }

    if(pbf) pb->terminate();
    if(ren_dir_fail)    return log_file;
    else                return "";
}



#endif
