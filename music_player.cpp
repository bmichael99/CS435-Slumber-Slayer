#include "mbed.h"
#include <chrono>

using namespace std;


class MusicPlayer {
private:
    //Define the frequency of basic music notes
    static constexpr float Do = 0.5;
    static constexpr float Re = 0.45;
    static constexpr float Mi = 0.4;
    static constexpr float Fa = 0.36;
    static constexpr float So = 0.33;
    static constexpr float La = 0.31;
    static constexpr float Si = 0.3;
    static constexpr float No = 0;

    //Define the beat length (e.g., whole note, half note)
    static constexpr float b1 = 0.5;
    static constexpr float b2 = 0.25;
    static constexpr float b3 = 0.125;
    static constexpr float b4 = 0.075;

    //Define the musical piece
    float note[49] = {Mi,No,Mi,No,Mi,No, Mi,No,Mi,No,Mi,No, Mi,No,So,No,Do,No,Re,No,Mi,No, Fa,No,Fa,No,Fa,No,Fa,No, Fa,No,Mi,No,Mi,No,Mi,No,Mi,No, Mi,Re,No,Re,Mi, Re,No,So,No};
    float beat[49] = {b3,b3,b3,b3,b2,b2, b3,b3,b3,b3,b2,b2, b3,b3,b3,b3,b3,b3,b3,b3,b2,b1, b3,b3,b3,b3,b3,b3,b3,b3, b3,b3,b3,b3,b3,b3,b4,b4,b4,b4, b2,b3,b3,b2,b2, b2,b2,b2,b2};

    //Define the PWM output for the speaker
    PwmOut PWM1;

    //Define the time ticker
    Ticker tick;

    //Static variable to iterate through note and beat arrays
    int k;

    //Timer ISR
    void timerISR() {
        tick.detach();

        if (note[k] == No) {
            PWM1 = 0;
        } else {
            PWM1.period(0.001 * note[k]);
            PWM1 = 0.5;
        }

        if (k >= 47) {
            k = 0;
        } else {
            k += 1;
        }

        tick.attach_us(callback(this, &MusicPlayer::timerISR), int(1000000*beat[k]));
    }

public:
    //constructor
    MusicPlayer(PinName pin) : PWM1(pin) {}

    //start music
    void startMusic() {
        k = 0;
        tick.attach_us(callback(this, &MusicPlayer::timerISR), 100000);
        
    }

    //stop music
    void stopMusic() {
        tick.detach();
        PWM1 = 0;
    }
};