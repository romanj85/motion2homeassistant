#include "motion.h"
#include "rest.h"

#include <iostream>
#include <cstdlib>

using namespace std;

int main(int argc, char* argv[]) {
    string webhookUrl;
    char* webhookPtr = std::getenv("MOTION_WEBHOOK");

    if (webhookPtr == nullptr) {
        if (argc != 2) {
            cout << "Usage:\n\t" << argv[0] << " <webhook url>\n\n"
                 << "or using enviroment variable:\n\tMOTION_WEBHOOK=<webhook url> " << argv[0] << endl;
            return 1;
        }
        webhookPtr = argv[1];
    }
    webhookUrl = string(webhookPtr);

    MotionDetect md;
    MotionRestCall rest(webhookUrl, std::chrono::seconds(2));
    rest.call("off");
    md.run(rest);

    return 0;
}
