#pragma once

#include "Arduino.h"

#include <Udp.h>

#define SEVENZYYEARS 2208988800UL
#define NTP_PACKET_SIZE 48
#define NTP_DEFAULT_LOCAL_PORT 1337

struct DateLanguageData {
    const char* shortWeekDays[7];
    const char* longWeekDays[7];
    const char* shortMonths[12];
    const char* longMonths[12];
};

const DateLanguageData EnglishData = {
    {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"},
    {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"},
    {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"},
    {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"}
};

const DateLanguageData PortugueseData = {
    {"Dom", "Seg", "Ter", "Qua", "Qui", "Sex", "Sáb"},
    {"Domingo", "Segunda-feira", "Terça-feira", "Quarta-feira", "Quinta-feira", "Sexta-feira", "Sábado"},
    {"Jan", "Fev", "Mar", "Abr", "Mai", "Jun", "Jul", "Ago", "Set", "Out", "Nov", "Dez"},
    {"Janeiro", "Fevereiro", "Março", "Abril", "Maio", "Junho", "Julho", "Agosto", "Setembro", "Outubro", "Novembro", "Dezembro"}
};

class NTPClient {
  private:
    UDP*          _udp;
    String        _dateLanguage = "en"; // Default language
    const char*   _poolServerName = "pool.ntp.org"; // Default time server
    IPAddress     _poolServerIP;
    unsigned int  _port           = NTP_DEFAULT_LOCAL_PORT;
    long          _timeOffset     = 0;

    unsigned long _updateInterval = 60000;  // In ms

    unsigned long _currentEpoc    = 0;      // In s
    unsigned long _lastUpdate     = 0;      // In ms
    unsigned long _lastRequest    = 0;      // In ms

    enum class State {
        uninitialized,
        idle,
        send_request,
        wait_response,
    } _state = State::uninitialized;

    byte          _packetBuffer[NTP_PACKET_SIZE];

    void          sendNTPPacket();

  public:
    NTPClient(UDP& udp);
    NTPClient(UDP& udp, long timeOffset);
    NTPClient(UDP& udp, const char* poolServerName);
    NTPClient(UDP& udp, const char* poolServerName, long timeOffset);
    NTPClient(UDP& udp, const char* poolServerName, long timeOffset, unsigned long updateInterval);
    NTPClient(UDP& udp, IPAddress poolServerIP);
    NTPClient(UDP& udp, IPAddress poolServerIP, long timeOffset);
    NTPClient(UDP& udp, IPAddress poolServerIP, long timeOffset, unsigned long updateInterval);


    /**
     * Starts the underlying UDP client with the default local port
     */
    void begin();

    /**
     * Starts the underlying UDP client with the specified local port
     */
    void begin(unsigned int port);

    /**
     * This should be called in the main loop of your application. By default an update from the NTP Server is only
     * made every 60 seconds. This can be configured in the NTPClient constructor.
     *
     * @return true on success, false on failure
     */
    bool update();

    /**
     * This will force the update from the NTP Server.
     *
     * @return true on success, false on failure
     */
    bool forceUpdate();

    /**
     * This allows to check if the NTPClient successfully received a NTP packet and set the time.
     *
     * @return true if time has been set, else false
     */
    bool isTimeSet() const;

    // Getters for time components
    /**
     * @return time in seconds since Jan. 1, 1970
     */
    unsigned long getEpochTime() const;
    int getDayOfWeek() const;
    int getHours() const;
    int getMinutes() const;
    int getSeconds() const;
    int getDay() const;
    int getMonth() const;
    int getYear() const;
    
    // Get formatted date and time
    String getFormattedDateTime(const String &format);
    
    // Setters components
    /**
     * Changes the time offset. Useful for changing timezones dynamically
     */
    void setTimeOffset(int timeOffset);
    /**
     * Set the update interval to another frequency. E.g. useful when the
     * timeOffset should not be set in the constructor
     */
    void setUpdateInterval(unsigned long updateInterval);
    /**
     * Set time server name
     *
     * @param poolServerName
     */
    void setPoolServerName(const char* poolServerName);
     /**
     * Set random local port
     */
    void setDateLanguage(const String &dateLanguage);
     /**
     * Set language for displaying date
     */
    void setRandomPort(unsigned int minValue = 49152, unsigned int maxValue = 65535);
     
    /**
     * Stops the underlying UDP client
     */
    void end();
};