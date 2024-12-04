```cpp
#include <iostream>
#include <fstream>
#include <vector>

void scan_binary_file(const std::string &filename, unsigned char target_byte) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Error opening file!" << std::endl;
        return;
    }

    // Read the entire file into a vector
    std::vector<unsigned char> buffer((std::istreambuf_iterator<char>(file)), {});
    file.close();

    size_t count = 0;
    size_t size = buffer.size();

    // Use inline assembly to count occurrences of target_byte
    asm volatile (
        "xor %%rcx, %%rcx;" // Clear count (rcx)
        "mov %1, %%al;"     // Load target_byte into AL
        "1:"                // Loop label
        "cmp %%rcx, %2;"    // Compare index (rcx) with size
        "jge 2f;"           // If index >= size, jump to end
        "movb (%3, %%rcx, 1), %%bl;" // Load byte from buffer into BL
        "cmp %%bl, %%al;"   // Compare byte with target_byte
        "jne 1b;"           // If not equal, continue looping
        "inc %%rdx;"        // Increment count (rdx)
        "inc %%rcx;"        // Increment index
        "jmp 1b;"           // Continue loop
        "2:"                // End label
        : "=d" (count)      // Output: count in rdx
        : "r" (target_byte), "r" (size), "r" (buffer.data()) // Inputs
        : "%rcx", "%al", "%bl" // Clobbered registers
    );

    std::cout << "Byte " << static_cast<int>(target_byte) << " found " << count << " times." << std::endl;
}

int main() {
    std::string filename;
    unsigned char target_byte;

    std::cout << "Enter the binary file name: ";
    std::cin >> filename;
    std::cout << "Enter the byte value to scan for (0-255): ";
    std::cin >> target_byte;

    scan_binary_file(filename, target_byte);

    return 0;
}
```
