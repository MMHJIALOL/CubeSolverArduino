#include <vector>
#include <string>
#include <iostream>
#include <map>
#include <cstdio>
#include <algorithm>
#include <sstream>

using namespace std;

void scancube() {
    string calcube = "python color.py"; 
    int sys1 = system(calcube.c_str());
    //the above line calls the color.py code which is used to extract each face of the cube

    FILE *fp = fopen("input.txt", "r");
    if (!fp) {
        cerr << "Error: Unable to open input.txt\n";
        return;
    }

    char res[300];
    fscanf(fp, "%s", res);
    fclose(fp);

    // Color mappings
    // W -> F (White -> Front)
    // G -> R (Green -> Right)
    // R -> U (Red -> Up)
    // O -> D (Orange -> Down)
    // B -> L (Blue -> Left)
    // Y -> B (Yellow -> Back)
    
    string s = "";
    int i = 0;
    while (res[i] != '\0') {
        if (res[i] == 'W') s += 'F';
        else if (res[i] == 'G') s += 'R';
        else if (res[i] == 'R') s += 'U';
        else if (res[i] == 'O') s += 'D';
        else if (res[i] == 'B') s += 'L';
        else if (res[i] == 'Y') s += 'B';
        i++;
    }

    FILE *fp1 = fopen("input2.txt", "w");
    fprintf(fp1, "%s", s.c_str());
    fclose(fp1);

    // Call kociemba algo solver
    int sys3 = system("python solver.py");
    FILE *fp2 = fopen("output.txt", "r");
    // char result[300];
    // fscanf(fp2, "%s", result);
    char result[300];
    if (fgets(result, sizeof(result), fp2) != nullptr) {
        cout << "Solution: " << result;
    }

    fclose(fp2);
}

int main() {
    scancube();
    return 0;
}
