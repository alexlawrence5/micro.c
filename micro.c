// i noticed that i writed variables complex in the asm edition.
// so i will name the variables, functions clean for this.
// please dont flag this project as vibecoded, it is not :-C.


// letzz setup the buffers here
char user[128];
char passw[256];
char input[256];
char choice[128];

#include "sys/kernelvars.h"
#include "sys/fetchvars.h"

typedef unsigned char u8;
typedef unsigned short u16;

#define VGA ((volatile u16 *)0xB8000)

static int row = 0;
static int col = 0;

static void putc(char c)
{
    if (c == '\n') {
        col = 0;
        row++;
        return;
    }

    if (c == '\b') {
        if (col > 0) {
            col--;
            VGA[row * 80 + col] = 0x0700;
        }
        return;
    }

    VGA[row * 80 + col] = 0x0700 | c;
    col++;

    if (col >= 80) {
        col = 0;
        row++;
    }
}

static void print(const char *s)
{
    while (*s)
        putc(*s++);
}

static void clear(void)
{
    for (int i = 0; i < 80 * 25; i++)
        VGA[i] = 0x0700;

    row = 0;
    col = 0;
}

static int strcmp(const char *a, const char *b)
{
    while (*a && *a == *b) {
        a++;
        b++;
    }

    return *a - *b;
}

static int starts_with(const char *s, const char *prefix)
{
    while (*prefix) {
        if (*s++ != *prefix++)
            return 0;
    }

    return 1;
}

static u8 inb(u16 port)
{
    u8 value;

    __asm__ volatile (
        "inb %1, %0"
        : "=a"(value)
        : "Nd"(port)
    );

    return value;
}

static int keyboard_ready(void)
{
    return inb(0x64) & 1;
}

static char keyboard_getchar(void)
{
    static const char keymap[128] = {
        0,  27,
        '1','2','3','4','5','6','7','8','9','0',
        '-','=','\b','\t',
        'q','w','e','r','t','y','u','i','o','p',
        '[',']','\n',0,
        'a','s','d','f','g','h','j','k','l',
        ';','\'','`',0,
        '\\','z','x','c','v','b','n','m',
        ',','.','/',0,
        '*',0,' '
    };

    while (!keyboard_ready())
        ;

    u8 scancode = inb(0x60);
    if (scancode & 0x80)
        return 0;

    if (scancode >= 128)
        return 0;

    return keymap[scancode];
}

static void readline(char *buffer, int max)
{
    int length = 0;

    while (1) {
        char c = keyboard_getchar();

        if (!c)
            continue;

        if (c == '\n') {
            putc('\n');
            buffer[length] = '\0';
            return;
        }

        if (c == '\b') {
            if (length > 0) {
                length--;
                putc('\b');
            }

            continue;
        }

        if (length < max - 1) {
            buffer[length++] = c;
            putc(c);
        }
    }
}

// app functions

void boot_ascii() {
    print("           _               \n");
    print(" _ __ ___ (_) ___ _ __ ___  \n");
    print("| '_ ` _ \\| |/ __| '__/ _ \\ \n");
    print("| | | | | | | (__| | | (_) |\n");
    print("|_| |_| |_|_|\\___|_|  \\___/ \n");
    print("Made with <3 by @turkishrevolt on TikTok.\n");
    print("");
}

void setup_wiz_microos() {
    print("Enter your all loving UNIX name: ");
    readline(user, 128);
    print("Enter your password for that user: ");
    readline(passw, 256);
    clear();
}

void setup_wiz_microos_choices() {
    print("Welcome to the MicroOS 1.8 Big Camel!\n");
    print("1. Set up your system\n");
    print("2. Halt the CPU\n");
    print("3. Reboot\n");
    print("Your choice: ");

    readline(choice, 128);
    
    if (strcmp(choice, "1") == 0) {
        clear();
        setup_wiz_microos();
    } else if (strcmp(choice, "2") == 0) {
        __asm__ volatile (
            "hlt"
        );
    } else if (strcmp(choice, "3") == 0) {
        __asm__ volatile (
            "cli\n"
            "mov $0xFE, %%al\n"
            "outb %%al, $0x64\n"
            :
            :
            : "eax"
        );
    } else if (choice[0] == '\0') {
        print("Invalid choice. rebooting!");
        __asm__ volatile (
                "cli\n"
                "mov $0xFE, %%al\n"
                "outb %%al, $0x64\n"
                :
                :
                :"eax"
            );
        } else if (choice[0] != '\0') {
            print("Invalid choice. rebooting!");
            __asm__ volatile (
                    "cli\n"
                    "mov $0xFE, %%al\n"
                    "outb %%al, $0x64\n"
                    :
                    :
                    :"eax"
            );
        }
    }

void micro_fetch() {
    print("OS: ");
    print(os);
    print("Kernel: ");
    print(kernel);
    print("CPU Platform: ");
    print(fetch_arch);
}

// end of app functions

static void shell(void)
{
    char command[128];

    setup_wiz_microos_choices();

    boot_ascii();

    while (1) {
        print(user);
        print(":/$ ");

        readline(command, sizeof(command));

        if (strcmp(command, "clear") == 0) {
            clear();
        }
        else if (starts_with(command, "echo ")) {
            print(command + 5);
            putc('\n');
        }
        else if (strcmp(command, "info") == 0) {
            print("MicroOS C Edition 1.8\n");
            print("Architecture: ");
            print(arch);
            print("Build type: ");
            print(build);
        }
        else if (strcmp(command, "microfetch") == 0) {
            micro_fetch();
        }

        else if (strcmp(command, "reboot") == 0) {
            print("this requires admin perms. for security reasons, we need you to enter your password: ");
            readline(input, 256);
            if (strcmp(input, passw) == 0) {
                print("rebooting...\n");

                __asm__ volatile (
                    "cli\n"
                    "mov $0xFE, %%al\n"
                    "outb %%al, $0x64\n"
                    :
                    :
                    : "eax"
                );
                }
            else {
                print("msh: invalid password\n");
            }
        }
        else if (command[0] != '\0') {
            print("msh: invalid command; ");
            print(command);
            putc('\n');
        }
    }
}

void kernel_main(void)
{
    clear();
    print("bmesg: MicroOS C\n");
    shell();
}

// after me writing this.. my brain fried when I patched some bugs.
