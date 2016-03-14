#include "Grinliz_Arduino_Util.h"
#include <Arduino.h>

LEDManager::LEDManager(uint8_t pin, bool on)
{
    m_pin = pin;
    m_on = on;
    digitalWrite(m_pin, m_on ? HIGH : LOW);
    pinMode(m_pin, OUTPUT);
}


void LEDManager::set(bool on)
{
    m_on = on;
    digitalWrite(m_pin, m_on ? HIGH : LOW );
    m_nBlink = 0;
}

void LEDManager::blink(uint8_t n, uint32_t cycle, BlinkHandler h)
{
    m_handler = 0;
    m_nCallbacks = 0;
    if (n == 0 || cycle == 0) {
        m_nBlink = 0;
        m_cycle = 1;
        m_startTime = 0;
        digitalWrite(m_pin, m_on ? HIGH : LOW );
    }
    else {
        m_handler = h;
        m_nBlink = n;
        m_cycle = cycle > 0 ? cycle : 1;
        m_startTime = millis();
    }
}

void LEDManager::process()
{
    if (m_nBlink) {
        uint32_t n = (millis() - m_startTime) / m_cycle;
        const uint32_t half = m_cycle / 2;
        uint32_t p = (millis() - m_startTime) / half;

        if (n > m_nBlink) {
            Serial.println("LED done");            
            m_nBlink = 0;
            digitalWrite(m_pin, m_on ? HIGH : LOW );
        }
        else {
            digitalWrite(m_pin, (p & 1) ? LOW : HIGH );
        }

        if (n >= m_nCallbacks) {
            Serial.print(" m_nBlink "); Serial.println(m_nBlink);
            if (m_handler) {
                m_handler(*this);
            }
            m_nCallbacks = n + 1;
            Serial.print("LED callback. m_nCallbacks "); Serial.print(m_nCallbacks); Serial.print(" n "); Serial.println(n);
            Serial.print("half "); Serial.print(half); Serial.print(" m_nBlink "); Serial.println(m_nBlink);
        }
    }
}

int LEDManager::numBlinks() const
{
    if (m_startTime == 0) return 0;

    uint32_t n = (millis() - m_startTime) / m_cycle;
    return n + 1;
}

bool strStarts(const char* str, const char* prefix)
{
    if (!str || !prefix)
        return false;

    while(*prefix) {
        if (*prefix++ != *str++)
            return false;
    }
    return true;
}

int hexToDec(char h) 
{
    if (h >= '0' && h <= '9') return h - '0';
    if (h >= 'a' && h <= 'f') return 10 + h - 'a';
    if (h >= 'A' && h <= 'F') return 10 + h - 'A';
    return 0;
}

char decToHex(int v) 
{
    if (v >= 0 && v < 10) return '0' + v;
    if (v>= 10 && v < 16) return 'a' + v - 10;
    return '0';
}


void parseNHex(const char* str, uint8_t* c, int n) 
{
    if (str[n] == 0 || str[n] == ' ' || str[n] == '\t') {
        // short form
        for (int i = 0; i < n; ++i) {
            int h = hexToDec(*str);
            c[i] =  h * 16 + h;
            ++str;
        }
    }
    else {
        for (int i = 0; i < n; ++i) {
            c[i] = hexToDec(*str) * 16;
            ++str;
            c[i] += hexToDec(*str);
            ++str;
        }
    }
}


void writeNHex(char* str, const uint8_t* c, int n) 
{
    for(int i=0; i<n; ++i) {
        str[i*2+0] = decToHex(c[i] / 16);
        str[i*2+1] = decToHex(c[i] & 15);
    }
    str[n*2] = 0;
}

bool Timer::tick()
{
    if (time == 0) {
        time = millis();  // start it up.
    }
    else {
        uint32_t delta = millis() - time;
        if (delta >= trigger) {
            time = millis();
            return true;
        }
    }
    return false;
}
