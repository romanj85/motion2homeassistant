#include "motion.h"
#include "rest.h"

#include <iostream>

using namespace std;
//using namespace restc_cpp;

int main() {
    MotionDetect md;
    MyRestCall rest(10s);
    rest.call("off");
    md.run(rest);
}
