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
    "showinfo"
};


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
//Custom strcpy function
void strcpy(char *dest, const char *src)
{
    while (*src)
    {
        *dest++ = *src++;
    }
    *dest = '\0';
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
//Board Revision

void print_board_revision(const unsigned int *mac) {
    if (mac = 0x00A02082)
    {
        uart_puts("rpi-3B BCM2837 1GiB Sony UK");
    }
    else if (mac = 0x00900092)
    {
        uart_puts("rpi-Zero BCM2835 512MB Sony UK");
    }
    else if (mac = 0x00000010)
    {
        uart_puts("rpi-1B+ BCM2835");
    }
    else if (mac = 0x00a01041)
    {
        uart_puts("rpi-2B BCM2836 1GiB Sony UK");
    }
    else
    {
        uart_puts("rpi-4B BCM2711 2GiB Sony UK");
    }
}
//Mac Address
void print_mac_address(const unsigned char *mac) {
    for (int i = 6; i >= 0; i--) {
        uart_dec(mac[i] % 16);  // Print last digit
        uart_dec(mac[i] / 16);  // Print second last digit
        if (i > 0) {
            uart_puts("-");
        }
    }
    uart_puts("\n");
}
void change_uart_driver (){

}
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
    mBuf[2] = 0x00010002; // TAG Identifier: Get board revision
    mBuf[3] = 4;          // Value buffer size in bytes
    mBuf[4] = 0;          // REQUEST CODE = 0
    mBuf[5] = 0;          // Clear output buffer 

    mBuf[6] = 0x00010003; // TAG Identifier: MAC address
    mBuf[7] = 6;          // Value buffer size in bytes
    mBuf[8] = 0;          // REQUEST CODE = 0
    mBuf[9] = 0;          // Clear output buffer         

    mBuf[11] = MBOX_TAG_LAST;


    if (mbox_call(ADDR(mBuf), MBOX_CH_PROP))
    {
        uart_puts("\nBoard Revision: ");
        print_board_revision(&mBuf[5]);
        // uart_hex(mBuf[5]);
        uart_puts("\nBoard MAC Address: ");
        // uart_hex(mBuf[9]);
        // uart_puts("\n");
        // uart_hex(mBuf[10]);
        print_mac_address((unsigned char*)&mBuf[9]);
    }
    else
    {
        uart_puts("Unable to query!\n");
    }
}

void cli()
{
    static char cli_buffer[MAX_CMD_SIZE];
    static int index = 0;

    // read and send back each char
    char c = uart_getc();
    uart_sendc(c);

    if (c != '\n' && c != '\b')
    {
        c != '\n' && c != '\b' && c != '\t';
        cli_buffer[index] = c;
        index++;
    }
    // Handle backspace
    if (c == '\b')
    {

        if (index <= 0)
        {
            uart_puts(" ");
            return;
        }
        uart_puts(" \b");
        index--;
        cli_buffer[index] = '\0';
    }

    //Command History (= first then -)
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

    // Handle newline  

    else if (c == '\n')
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