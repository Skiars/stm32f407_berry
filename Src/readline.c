#include "readline.h"
#include "berry.h"
#include <ctype.h>
#include <string.h>

#define LINE_LENGTH     64
#define HISTORY_COUNT   8

#define QUEUE_SIZE      16
#define QUEUE_EMPTY     0x10000

#define queue_next(queue, attr) \
    ((queue)->attr < QUEUE_SIZE - 1 ? (queue)->attr + 1 : 0)

struct queue {
    volatile int front, rear;
    char array[QUEUE_SIZE];
};

struct shell {
    int linepos, linecount, newline;
    int hiscur, hiscnt;
    char line[LINE_LENGTH + 1];
    char history[HISTORY_COUNT][LINE_LENGTH + 1];
    struct queue queue;
    const char *prompt;
};

static struct shell m_shell;

#define queue_isempty(queue)    ((queue)->front == (queue)->rear)

static int queue_push(struct queue *queue, int ch)
{
    int next = queue_next(queue, front);
    if (next != queue->rear) { /* not full */
        queue->array[queue->front] = (char)ch;
        queue->front = next;
        return 0;
    }
    return 1;
}

static int queue_pop(struct queue *queue)
{
    if (!queue_isempty(queue)) {
        int res = queue->array[queue->rear];
        queue->rear = queue_next(queue, rear);
        return res;
    }
    return QUEUE_EMPTY;
}

static int wait_char(struct shell *shell)
{
    int ch;
    while ((ch = queue_pop(&shell->queue)) ==  QUEUE_EMPTY);
    return ch;
}

void pack_line(struct shell *shell)
{
    shell->line[shell->linecount] = '\0';
    if (shell->linecount) {
        int hiscur = shell->hiscnt % HISTORY_COUNT;
        strcpy(shell->history[hiscur], shell->line);
        shell->hiscur = ++shell->hiscnt;
    }
    shell->linecount = shell->linepos = 0;
}

static void remove_char(struct shell *shell)
{
    int pos = --shell->linepos, count = shell->linecount;
    while (pos < count) {
        shell->line[pos] = shell->line[pos + 1];
        ++pos;
    }
    shell->line[--shell->linecount] = '\0';
}

static void insert_char(struct shell *shell, int ch)
{
    int pos = shell->linepos, count = shell->linecount;
    if (count < LINE_LENGTH) {
        while (count >= pos) {
            shell->line[count + 1] = shell->line[count];
            --count;
        }
        shell->line[pos] = ch;
        ++shell->linecount;
        ++shell->linepos;
        for (count = shell->linecount; pos < count; ++pos) {
            be_writebuffer(&shell->line[pos], 1);
        }
        for (count -= shell->linepos; count > 0; --count) {
            be_writestring("\b");
        }
    }
}

static void backspace(struct shell *shell)
{
    if (shell->linepos > 0) {
        remove_char(shell);
        if (shell->linepos < shell->linecount) {
            int i;
            be_writestring("\b");
            be_writestring(shell->line + shell->linepos);
            be_writestring(" \b");
            for (i = shell->linepos; i < shell->linecount; ++i) {
                be_writestring("\b");
            }
        } else {
            be_writestring("\b \b");
        }
    }
}

static int addchar(struct shell *shell, int ch)
{
    switch (ch) {
    case '\r':
        pack_line(shell);
        be_writestring("\n");
        shell->newline = 1;
        return 1;
    case '\n':
        if (shell->newline) {
            shell->newline = 0;
            break;
        }
        pack_line(shell);
        be_writestring("\n");
        return 1;
    case '\b': case '\177':
        backspace(shell);
        break;
    default:
        if (isprint(ch)) {
            insert_char(shell, ch);
        }
    }
    return 0;
}

static void change_history(struct shell *shell)
{
    int pos = shell->hiscur % HISTORY_COUNT;
    char *line = shell->history[pos];
    strcpy(shell->line, line);
    be_writestring("\r");
    be_writestring(shell->prompt);
    be_writestring(line);
    be_writestring("\033[K");
    shell->linecount = shell->linepos = strlen(line);
}

static void up_key(struct shell *shell)
{
    if (shell->hiscur > 0 && shell->hiscur
            + HISTORY_COUNT > shell->hiscnt) {
        --shell->hiscur;
        change_history(shell);
    }
}

static void down_key(struct shell *shell)
{
    if (shell->hiscur + 1 < shell->hiscnt) {
        ++shell->hiscur;
        change_history(shell);
    }
}

static void left_key(struct shell *shell)
{
    if (shell->linepos > 0) {
        --shell->linepos;
        be_writestring("\b");
    }
}

static void right_key(struct shell *shell)
{
    if (shell->linepos < shell->linecount) {
        be_writebuffer(&shell->line[shell->linepos++], 1);
    }
}

const char* p_readline(struct shell *shell)
{
    int state = 0;
    for (;;) {
        int ch = wait_char(shell);
        if (ch == 0x1B) {
            state = 1;
        } else if (state == 1) {
            state = ch == 0x5B ? 2 : 0;
        } else if (state == 2) {
            state = 0;
            switch (ch) {
            case 0x41: up_key(shell); break;
            case 0x42: down_key(shell); break;
            case 0x43: right_key(shell); break;
            case 0x44: left_key(shell); break;
            default: break;
            }
        } else if (addchar(shell, ch)) {
            return shell->line;
        }
        shell->newline = 0;
    }
}

int readline_putchar(int ch)
{
    queue_push(&m_shell.queue, ch);
    return ch;
}

const char* readline(const char *prompt)
{
    m_shell.prompt = prompt;
    be_writestring(prompt);
    return p_readline(&m_shell);
}

int readchar(void)
{
    return wait_char(&m_shell);
}
