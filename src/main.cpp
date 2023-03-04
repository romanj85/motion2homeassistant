#include "motion.h"
#include "rest.h"

#include <iostream>
#include <cstdlib>

using namespace std;

int main(int argc, char* argv[]) {
    string webhookUrl;
    char* webhookPtr = std::getenv("MOTION_WEBHOOK");
    char* skipPeriodPtr = std::getenv("MOTION_SKIP_PERIOD");
    int skipPeriod = 2;

    if (!webhookPtr) {
        if (argc < 2) {
            cout << "Usage:\n\t" << argv[0] << " <webhook url> [skip period seconds]\n\n"
                 << "or using enviroment variable:\n\tMOTION_WEBHOOK=<webhook url> MOTION_SKIP_PERIOD=[seconds] " << argv[0] << endl;
            return 1;
        }
        webhookPtr = argv[1];
    }
    webhookUrl = string(webhookPtr);
    if (!skipPeriodPtr) {
        if (argc == 3) {
            skipPeriod = std::atoi(argv[2]);
        }
    } else {
        skipPeriod = std::atoi(skipPeriodPtr);
    }

    MotionDetect md;
    MotionRestCall rest(webhookUrl, std::chrono::seconds(skipPeriod));
    rest.call("off");
    md.run(rest);

    return 0;
}
