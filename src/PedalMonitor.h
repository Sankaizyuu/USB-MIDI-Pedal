#ifndef PedalMonitor_h
#define PedalMonitor_h

#include<Arduino.h>

class PedalMonitor{
  public:
    PedalMonitor(const int* pins, int numSwitches, unsigned long debounceDelay = 50);

    // 各スイッチの状態をチェックし、変化があればコールバックを呼び出す
    void update();

    // コールバック関数を設定
    void setCallback(void (*callback)(int, int));

  private:
    const int* _pins;
    int _numSwitches;
    int* _currentStates;
    int* _previousStates;
    unsigned long* _lastChangeTimes; // 最後に状態が変わった時間
    unsigned long _debounceDelay;    // デバウンス時間
    void (*_callback)(int, int);  // コールバック関数のポインタ

};

#endif