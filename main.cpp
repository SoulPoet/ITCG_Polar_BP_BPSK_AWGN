#include "CommonFunc.hpp"
#include "Simulator.hpp"

int main() {
    Simulator s;
    const char *config_file_path = "Simulator.ini";
    s.Init(config_file_path);
    s.Start();
    //s.DisplayResult();
    s.Clean();
    return 0;
}
