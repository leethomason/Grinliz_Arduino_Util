#include "Grinliz_Arduino_Util.h"
#include <Arduino.h>

SPClass SPrint;

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

void LEDManager::blink(uint8_t n, uint32_t cycle, BlinkHandler h, uint8_t bias)
{
    //SPrint.p("blink n=").p(n).p(" cycle=").p(cycle).eol();

    m_handler = 0;
    m_nCallbacks = 0;
    m_bias = bias;
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

        if (n >= m_nBlink) {
            m_nBlink = 0;
            digitalWrite(m_pin, m_on ? HIGH : LOW );
        }
        else {
            digitalWrite(m_pin, ((p & 1) == m_bias) ? LOW : HIGH );

            if (n >= m_nCallbacks) {
                if (m_handler) {
                    m_handler(*this);
                }
                m_nCallbacks = n + 1;
                //SPrint.p("LED callback. m_nCallbacks=").p(m_nCallbacks).p(" n=").p(n).eol();
            }
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

void strBufCpy(char* target, int targetBufSize, const char* src)
{
    if (!target || !src) return;

    int i = 0;
    for(; (i < targetBufSize - 1) && src[i]; ++i) {
      target[i] = src[i];
    }
    target[i] = 0;
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


void SPLog::attachSerial(Stream* stream) 
{
    serialStream = stream;
}

void SPLog::attachLog(Stream* stream)
{
    logStream = stream;
}

const SPLog& SPLog::p(const char v[]) const
{
  if (serialStream)
      serialStream->print(v);
  if (logStream)
      logStream->print(v);
  return *this;  
}
 
const SPLog& SPLog::p(char v) const
{
  if (serialStream)
      serialStream->print(v);
  if (logStream)
      logStream->print(v);
  return *this;  
}

const SPLog& SPLog::p(unsigned char v, int p) const
{
  if (serialStream)
      serialStream->print(v, p);
  if (logStream)
      logStream->print(v, p);
  return *this;  
}

const SPLog& SPLog::p(int v, int p) const
{
  if (serialStream)
      serialStream->print(v, p);
  if (logStream)
      logStream->print(v, p);
  return *this;  
}

const SPLog& SPLog::p(unsigned int v, int p) const
{
  if (serialStream)
      serialStream->print(v, p);
  if (logStream)
      logStream->print(v, p);
  return *this;  
}

const SPLog& SPLog::p(long v, int p) const
{
  if (serialStream)
      serialStream->print(v, p);
  if (logStream)
      logStream->print(v, p);
  return *this;  
}

const SPLog& SPLog::p(unsigned long v, int p) const
{
  if (serialStream)
      serialStream->print(v, p);
  if (logStream)
      logStream->print(v, p);
  return *this;  
}

const SPLog& SPLog::p(double v, int p) const
{
  if (serialStream)
      serialStream->print(v, p);
  if (logStream)
      logStream->print(v, p);
  return *this;  
}

void SPLog::eol() const
{
  if (serialStream)
      serialStream->println("");
  if (logStream) {
      logStream->println("");
      logStream->flush();
    }
}

void SPLog::event(const char* e, const char* d)
{
  eventCache = e;
  eventStacked = true;

  dataCache.clear();
  if (d) {
    dataCache = d;
    p(eventCache.c_str()).p(" ").p(dataCache.c_str()).eol();
  }
  else {
    p(eventCache.c_str()).eol();
  }
}

const char* SPLog::popEvent(const char** d)
{
  if (d)
    *d = 0;
  if (eventStacked) {
    eventStacked = false;    
    if (d) {
      *d = dataCache.c_str();
    }
    return eventCache.c_str();
  }
  return 0;
}

SPLog Log;
