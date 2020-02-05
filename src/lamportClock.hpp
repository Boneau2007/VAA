//
// Created by alhuber on 04.02.20.
//

#ifndef VAA_LAMPORTCLOCK_HPP
#define VAA_LAMPORTCLOCK_HPP

#include <atomic>

class LamportClock{
private:
    std::atomic<unsigned int> time;
public:
    //Constructors
    LamportClock() : time(0) {}
    LamportClock(const LamportClock& clock){
        time = clock.getTime();
    }

    //Inline-Elementfunctions
    unsigned int getTime() const { return time.load(); }

    //Memberfunctions

    //Increment local timer if local event happend
    unsigned int localEvent() { return time.fetch_add(1); }
    //Inrement local timer if send event happend
    unsigned int sendEvent() { return time.fetch_add(1); }

    //Handles incomming events
    //Sets the recv time and sets the local time to recv time plus 1
    unsigned int recvEvent(unsigned int recvTime){
        RECV_EVENT:
        auto current = getTime();
        // If recv time is old do nothing
        if(recvTime < current){
            return current;
        }
        //Ensure that the local time it at least one ahead
        if(!time.compare_exchange_strong(current, recvTime+1)){
            goto RECV_EVENT;
        }
        return recvTime+1;
    }
};



#endif //VAA_LAMPORTCLOCK_HPP
