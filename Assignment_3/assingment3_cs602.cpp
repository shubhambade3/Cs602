#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
using namespace std;

int main()
{
    ifstream file("assi3_input.asm");

    if (!file)
    {
        cout << "File not found!" << endl;
        return 0;
    }

    string line;
    string section = "";

    int address = 0;

    cout << "Address\tHexadecimal\tMain Code" << endl;
    cout << "---------------------------------------------" << endl;

    while (getline(file, line))
    {
        stringstream ss(line);

        string word1, word2;
        int value;

        ss >> word1;

        if (word1 == "")
            continue;

        // Check section
        if (word1 == "section")
        {
            ss >> section;
            continue;
        }

        // Only process .data section
        if (section == ".data")
        {
            ss >> word2 >> value;

            int size = 0;

            // DB
            if (word2 == "db")
            {
                size = 1;
            }

            // DW
            else if (word2 == "dw")
            {
                size = 2;
            }

            // DD
            else if (word2 == "dd")
            {
                size = 4;
            }

            // DQ
            else if (word2 == "dq")
            {
                size = 8;
            }

            else
            {
                continue;
            }

            // Print address
            cout << setfill('0') << setw(4)
                 << hex << uppercase << address;

            // Print hexadecimal value
            cout << "\t";

            int temp = value;

            for (int i = 0; i < size; i++)
            {
                cout << setfill('0') << setw(2)
                     << hex << uppercase
                     << (temp & 0xFF) << " ";

                temp = temp >> 8;
            }

            // Print main code
            cout << "\t" << word1 << " "
                 << word2 << " "
                 << dec << value << endl;

            // Increase address
            address = address + size;
        }

        // .text section
        else if (section == ".text")
        {
            cout << setfill('0') << setw(4)
                 << hex << uppercase << address;

            cout << "\t";

            cout << "-" << "\t\t";

            cout << line << endl;
        }
    }

    file.close();

    return 0;
}
