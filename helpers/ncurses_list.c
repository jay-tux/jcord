#include <ncurses.h>

int main()
{
    for(int i = 0; i <= 411; i++)
    {
        printf("KEY %d: %s.\n", i, keyname(i));
    }
}
