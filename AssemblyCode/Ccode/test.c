// Simple test of stack, function calls, and loops
int add_range(int n)
{
    if (n == 0) return 0;
    return add_range(n - 1) + n;
}

int main()
{
    int x = 100;
    int result = add_range(x); // should compute 1+2+3+4+5 = 15

    // put result into MMIO exit register
    volatile unsigned int *MMIO_EXIT = (unsigned int *)0x8F000100;
    *MMIO_EXIT = result; // emulator should halt with 5050

    return 0;
}
