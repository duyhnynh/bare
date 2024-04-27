#include "uart.h"
#include "color.h"
#include "mbox.h"
#define MAX_COMMANDS 4
#define MAX_CMD_SIZE 100
#define OS_NAME "DuyOS"
char *history[100][100];
static int historyList = 0;
static int currentHistoryList = 0;
const char *Commands[MAX_COMMANDS] =
    {
        "help",
        "clear",
        "setcolor",
        "showinfo"};

// -----------------------------------customed functions -------------------------------------

// Custom strcmp function
int strcmp(const char *s1, const char *s2)
{
    while (*s1 && (*s1 == *s2))
    {
        s1++;
        s2++;
    }
    return *(const unsigned char *)s1 - *(const unsigned char *)s2;
}
// Custom strncmp function
int strncmp(const char *s1, const char *s2, int n)
{
    while (n-- && *s1 && *s2)
    {
        if (*s1 != *s2)
        {
            return (*(unsigned char *)s1 - *(unsigned char *)s2);
        }
        s1++;
        s2++;
    }
    return 0;
}
int strncmp2(const char *s1, const char *s2, int n)
{
    for (int i = 0; i < n; i++)
    {
        if (s1[i] != s2[i])
        {
            return 0;
        }
    }
    return 1;
}

// Custom strlen function
int strlen(const char *str)
{
    int len = 0;
    while (str[len] != '\0')
    {
        len++;
    }
    return len;
}
// Custom strcpy function
void strcpy(char *dest, const char *src)
{
    while (*src)
    {
        *dest++ = *src++;
    }
    *dest = '\0';
}

void memset(void *str, int startIndex, int index)
{
    unsigned char *p = (unsigned char *)str;
    while (index--)
    {
        *p++ = (unsigned char)startIndex;
    }
}
void print_available_commands()
{
    uart_puts("Available commands:\n");
    uart_puts("---------------------------------------------------------------------\n");
    uart_puts("| No. | Command              | Usage                                 |\n");
    uart_puts("---------------------------------------------------------------------\n");
    uart_puts("| 1.  | help                 | Show brief information of all commands|\n");
    uart_puts("| 2.  | help <command_name>  | Show full information of the command  |\n");
    uart_puts("| 3.  | clear                | Clear screen                          |\n");
    uart_puts("| 4.  | setcolor             | Set text and/or background color      |\n");
    uart_puts("| 5.  | showinfo             | Show board revision and MAC address   |\n");
    uart_puts("---------------------------------------------------------------------\n");
};

// -----------------------------------main functions -------------------------------------

// Help function
void help_function(char *command_name)
{
    // Show full information for the specified command
    if (strcmp(command_name, "help") == 0)
    {
        uart_puts("Description: Show brief information of all commands or full information of a specific command.\n");
    }
    else if (strcmp(command_name, "clear") == 0)
    {
        uart_puts("Description: Clear screen (in our terminal it will scroll down to current position of the cursor).\n");
    }
    else if (strcmp(command_name, "setcolor") == 0)
    {
        uart_puts("Description: Set text and/or background color.\n");
    }
    else if (strcmp(command_name, "showinfo") == 0)
    {
        uart_puts("Description: Show board revision and MAC address.\n");
    }
    else
    {
        // uart_puts("Invalid command\n");
        print_available_commands();
    }
}
// Clear Screen Function
void clear_screen()
{
    uart_puts("\e[1;1H\e[2J"); // ANSI escape code to clear the screen
}
// Setcolor Function
void setcolor(char *color_name)
{
    uart_puts("\nChoose which color(s) to change (t = text, b = background, or a = both): \n");
    char choice = uart_getc(); // Read a single character from user input

    // Define variables to store the ANSI escape sequences for text and background colors
    char *text_color = "";
    char *bg_color = "";

    switch (choice)
    {
    case 't':
        // Change text color
        uart_puts("Choose text color (b for black, r for red, g for green, y for yellow, l for blue, m for magenta, c for cyan, w for white): ");
        choice = uart_getc(); // Read a single character from user input
        switch (choice)
        {
        case 'b':
            text_color = COLOR_BLACK; // Set text color to black
            break;
        case 'r':
            text_color = COLOR_RED; // Set text color to red
            break;
        case 'g':
            text_color = COLOR_GREEN; // Set text color to green
            break;
        case 'y':
            text_color = COLOR_YELLOW; // Set text color to yellow
            break;
        case 'l':
            text_color = COLOR_BLUE; // Set text color to blue
            break;
        case 'm':
            text_color = COLOR_MAGENTA; // Set text color to magenta
            break;
        case 'c':
            text_color = COLOR_CYAN; // Set text color to cyan
            break;
        case 'w':
            text_color = COLOR_WHITE; // Set text color to white
            break;
        default:
            uart_puts("\nInvalid text color. Please choose again.\n");
            return;
        }
        break;
    case 'b':
        // Change background color
        uart_puts("Choose background color (b for black, r for red, g for green, y for yellow, l for blue, m for magenta, c for cyan, w for white): ");
        choice = uart_getc(); // Read a single character from user input
        switch (choice)
        {
        case 'b':
            bg_color = BG_BLACK; // Set background color to black
            break;
        case 'r':
            bg_color = BG_RED; // Set background color to red
            break;
        case 'g':
            bg_color = BG_GREEN; // Set background color to green
            break;
        case 'y':
            bg_color = BG_YELLOW; // Set background color to yellow
            break;
        case 'l':
            bg_color = BG_BLUE; // Set background color to blue
            break;
        case 'm':
            bg_color = BG_MAGENTA; // Set background color to magenta
            break;
        case 'c':
            bg_color = BG_CYAN; // Set background color to cyan
            break;
        case 'w':
            bg_color = BG_WHITE; // Set background color to white
            break;
        default:
            uart_puts("\nInvalid background color. Please choose again.\n");
            return;
        }
        break;
    case 'a':
        // Change both text and background colors
        uart_puts("Choose text color (b for black, r for red, g for green, y for yellow, l for blue, m for magenta, c for cyan, w for white): ");
        choice = uart_getc(); // Read a single character from user input
        switch (choice)
        {
        case 'b':
            text_color = COLOR_BLACK; // Set text color to black
            break;
        case 'r':
            text_color = COLOR_RED; // Set text color to red
            break;
        case 'g':
            text_color = COLOR_GREEN; // Set text color to green
            break;
        case 'y':
            text_color = COLOR_YELLOW; // Set text color to yellow
            break;
        case 'l':
            text_color = COLOR_BLUE; // Set text color to blue
            break;
        case 'm':
            text_color = COLOR_MAGENTA; // Set text color to magenta
            break;
        case 'c':
            text_color = COLOR_CYAN; // Set text color to cyan
            break;
        case 'w':
            text_color = COLOR_WHITE; // Set text color to white
            break;
        default:
            uart_puts("\nInvalid text color. Please choose again.\n");
            return;
        }

        uart_puts("Choose background color (b for black, r for red, g for green, y for yellow, l for blue, m for magenta, c for cyan, w for white): ");
        choice = uart_getc(); // Read a single character from user input
        switch (choice)
        {
        case 'b':
            bg_color = BG_BLACK; // Set background color to black
            break;
        case 'r':
            bg_color = BG_RED; // Set background color to red
            break;
        case 'g':
            bg_color = BG_GREEN; // Set background color to green
            break;
        case 'y':
            bg_color = BG_YELLOW; // Set background color to yellow
            break;
        case 'l':
            bg_color = BG_BLUE; // Set background color to blue
            break;
        case 'm':
            bg_color = BG_MAGENTA; // Set background color to magenta
            break;
        case 'c':
            bg_color = BG_CYAN; // Set background color to cyan
            break;
        case 'w':
            bg_color = BG_WHITE; // Set background color to white
            break;
        default:
            uart_puts("\nInvalid background color. Please choose again.\n");
            return;
        }
        break;
    default:
        // Invalid choice
        uart_puts("Invalid choice. Please select 't', 'b', or 'a'.\n");
        return;
    }

    // Apply the chosen colors
    uart_puts(text_color);
    uart_puts(bg_color);
}

void showinfo()

{
    mBuf[0] = 12 * 4;
    mBuf[1] = MBOX_REQUEST;
    // get board revision
    mBuf[2] = 0x00010002; // tag identifier : board revision
    mBuf[3] = 4;          // max response length
    mBuf[4] = 0;          // request code
    mBuf[5] = 0;          // clear

    mBuf[6] = 0x00010003; // tag identifier : MAC address
    mBuf[7] = 6;
    mBuf[8] = 0; // request code
    mBuf[9] = 0;
    mBuf[10] = 0;

    mBuf[11] = MBOX_TAG_LAST;

    // Note: Board model and Board serial may give 0 values on QEMU.
    if (mbox_call(ADDR(mBuf), MBOX_CH_PROP))
    {
        uart_puts("\nBoard Revision: ");
        uart_hex(mBuf[5]);
        uart_puts("\nBoard MAC Address: ");
        uart_hex(mBuf[9]);
        uart_puts("\n");
        uart_hex(mBuf[10]);
    }
    else
    {
        uart_puts("Unable to query!\n");
    }
}

char *auto_completion(char cli_buffer[], int buffer_index)
{
    const char *validCommand[] = {
        "help", "setcolor", "clear", "showinfo"};
    for (int i = 0; i < 4; i++)
    {
        if (strncmp2(cli_buffer, validCommand[i], buffer_index) == 1)
        {
            for (int j = buffer_index; j > 0; j--)
            {
                uart_sendc('\b');
                uart_sendc(' ');
                uart_sendc('\b');
            }
            return (char *)validCommand[i];
        }
    }
    return 0;
}

void cli()
{
    static char cli_buffer[MAX_CMD_SIZE];
    static int index = 0;

    // read and send back each char
    char c = uart_getc();

    if (c != '\n' && c != '\b' && c != '\t')
    {
        uart_sendc(c);
        // uart_puts("Hello world");
        cli_buffer[index] = c;
        index++;
    }
    // Handle backspace
    if (c == '\b')
    {

        if (index > 0)
        {
            index--;
            cli_buffer[index] = '\0';
            uart_sendc('\b');
            uart_sendc(' ');
            uart_sendc('\b');
        }
    }

    // Command History (= first then -)
    if (c == 45) // (-)
    {
        if (currentHistoryList <= 0)
        {
            return;
        }
        currentHistoryList--;
        for (int i = 0; i < index; i++)
        {
            uart_puts("\b \b");
        }
        strcpy(cli_buffer, history[currentHistoryList]);
        index = strlen(cli_buffer);
        uart_puts(history[currentHistoryList]);
    }
    if (c == 61) // (=)
    {

        if (currentHistoryList > historyList)
        {
            return;
        }
        currentHistoryList++;
        for (int i = 0; i < index; i++)
        {
            uart_puts("\b \b");
        }
        strcpy(cli_buffer, history[currentHistoryList]);
        index = strlen(cli_buffer);
        uart_puts(history[currentHistoryList]);
    }
    if (c == '\t')
    {

        char *completedCommand = auto_completion(cli_buffer, index);
        index = strlen(completedCommand);
        memset(cli_buffer, 0, index);
        strcpy(cli_buffer, completedCommand);
        uart_puts(cli_buffer);
    }

    // Handle newline

    if (c == '\n')
    {
        // Terminate the buffer
        cli_buffer[index] = '\0';
        if (index == 0)
        {
            // Print the prompt and return without executing any command
            uart_puts("\n");
            uart_puts(OS_NAME);
            uart_puts(":> ");
            return;
        }

        // Process the completed command
        uart_puts("\nGot commands: ");
        uart_puts(cli_buffer);
        strcpy(history[historyList], cli_buffer);
        currentHistoryList = 0;
        historyList++;
        uart_puts("\n");

        // Check if the command is "help"
        if (strncmp(cli_buffer, "help", 4) == 0)
        {
            char *helpExtra;
            for (int i = 0; i < strlen(cli_buffer); i++)
            {
                helpExtra[i] = cli_buffer[i + 5];

                if (cli_buffer[i + 5] == '\0')
                {
                    helpExtra[i] = '\0';
                }
            }
            help_function(helpExtra);
        }

        // Check if the command is "clear"
        else if (strcmp(cli_buffer, "clear") == 0)
        {
            clear_screen();
        }
        else if (strcmp(cli_buffer, "setcolor") == 0)
        {
            setcolor("default"); // Call setcolor function with a default color name
        }
        else if (strcmp(cli_buffer, "showinfo") == 0)
        {
            showinfo();
        }
        else if (strcmp(cli_buffer, "change") == 0) // change baudrates 9600
        {
            showinfo();
        }
        else
        {
            uart_puts("Invalid command!");
        }
        // //Clear buffer
        for (int i = index; i >= 0; i--)
        {
            cli_buffer[i] = '\0';
        }
        index = 0;
        uart_puts("\n");
        uart_puts(OS_NAME);
        uart_puts(":> ");
    }
}

void main()
{
    // set up serial console
    uart_init();

    // Print welcome message
    uart_puts(
        " ______ ______ ______ _______ ___  _  _   ___   ___  	\n"
        "|  ____|  ____|  ____|__   __|__ \\| || | / _ \\ / _ \\ 	\n"
        "| |__  | |__  | |__     | |     ) | || || (_) | | | |	\n"
        "|  __| |  __| |  __|    | |    / /|__   _\\__, | | | |	\n"
        "| |____| |____| |____   | |   / /_   | |   / /| |_| |	\n"
        "|______|______|______|  |_|  |____|  |_|  /_/  \\___/ 	\n"
        "\n"
        " ____          _____  ______    ____   _____    \n"
        "|  _ \\   /\\   |  __ \\|  ____|  / __ \\ / ____|	\n"
        "| |_) | /  \\  | |__) | |__    | |  | | (___  	\n"
        "|  _ < / /\\ \\ |  _  /|  __|   | |  | |\\___ \\ 	\n"
        "| |_) / ____ \\| | \\ \\| |____  | |__| |____) |	\n"
        "|____/_/    \\_\\_|  \\_\\______|  \\____/|_____/ 	\n"
        "\n"
        "Developed by Huynh Ngoc Duy - s3924704 \n");

    // //Print OS line
    uart_puts("\n");
    uart_puts(OS_NAME);
    uart_puts(":> ");
    // run CLI
    while (1)
    {
        cli();
    }
}