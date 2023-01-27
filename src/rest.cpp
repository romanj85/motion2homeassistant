#include "rest.h"

#include "restc-cpp/restc-cpp.h"
#include "restc-cpp/RequestBuilder.h"

#include <boost/fusion/adapted.hpp>

#include <iostream>
#include <chrono>
#include <iomanip>

using namespace std;
using namespace restc_cpp;

// C++ structure that match the JSON entries received
// from http://jsonplaceholder.typicode.com/posts/{id}
struct Post {
    //int userId = 0;
    string motion;
};

// Since C++ does not (yet) offer reflection, we need to tell the library how
// to map json members to a type. We are doing this by declaring the
// structs/classes with BOOST_FUSION_ADAPT_STRUCT from the boost libraries.
// This allows us to convert the C++ classes to and from JSON.

BOOST_FUSION_ADAPT_STRUCT(
    Post,
    //(int, userId)
    (string, motion)
)

bool MyRestCall::call(string motionState) {
    std::time_t t = time(nullptr);

    if (1 || chrono::system_clock::now() < _nextFire) {
        clog << std::put_time(std::localtime(&t), "%c %Z") << " skipping Rest::call()\n";
        return false;
    }
    _nextFire = chrono::system_clock::now() + _skipPeriod;
    clog << std::put_time(std::localtime(&t), "%c %Z") << " Rest::call(" << motionState << ")\n";

    // Construct our own ioservice.
    boost::asio::io_service ioservice;

    // Give it some work so it don't end prematurely
    boost::asio::io_service::work work(ioservice);

    // Start it in a worker-thread
    thread worker([&ioservice]() {
        ioservice.run();
    });

    auto rest_client = RestClient::Create(ioservice);

    rest_client->ProcessWithPromise([&](Context& ctx) {
        // Here we are in a co-routine, spawned from our io-service, running
        // in our worker thread.

        try {
            Post data_object = {motion: motionState};
            // Asynchronously connect to a server and fetch some data.
            auto reply = RequestBuilder(ctx)
                .Post("http://192.168.88.215:30123/api/webhook/baby-motion-DTY")              // URL
                .Header("X-Client", "RESTC_CPP")                   // Optional header
                //.BasicAuthentication("alice", "12345")
                .Data(data_object)                                 // Data object to send
                .Execute();
            // Asynchronously fetch the entire data-set and return it as a string.
            auto json = reply->GetBodyAsString();

            // Just dump the data.
            //clog << "Received data: " << json << endl;
        } catch (const boost::exception& ex) {
            clog << "Caught boost exception: "
                << boost::diagnostic_information(ex)
                << endl;
        } catch (const exception& ex) {
            clog << "Caught exception: "
                << ex.what()
                << endl;
        }
    })
    // Wait for the co-routine to end
    .get();

    // Stop the io-service
    // (We can not just remove 'work', as the rest client may have
    // timers pending in the io-service, keeping it running even after
    // work is gone).
    ioservice.stop();

    // Wait for the worker thread to end
    worker.join();

    return true;
}
