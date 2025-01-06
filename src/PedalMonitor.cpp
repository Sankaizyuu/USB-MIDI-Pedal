#include "PedalMonitor.h"

// コンストラクタ
PedalMonitor::PedalMonitor(const int* pins, int numSwitches, unsigned long debounceDelay) 
    : _pins(pins), _numSwitches(numSwitches), _debounceDelay(debounceDelay) {
    
    // スイッチの状態を保持する配列を初期化
    _currentStates = new int[_numSwitches];
    _previousStates = new int[_numSwitches];
    _lastChangeTimes = new unsigned long[_numSwitches]; // デバウンス用の時間配列
    
    // スイッチのピンを設定し、初期状態を読み込む
    for (int i = 0; i < _numSwitches; i++) {
        pinMode(_pins[i], INPUT_PULLUP);
        _currentStates[i] = digitalRead(_pins[i]);
        _previousStates[i] = _currentStates[i];
        _lastChangeTimes[i] = 0; // 初期化
    }
    
    _callback = nullptr;  // コールバック関数を初期化
}

// 状態チェックとコールバックの呼び出し
void PedalMonitor::update() {
    for (int i = 0; i < _numSwitches; i++) {
        int reading = digitalRead(_pins[i]);
        unsigned long currentTime = millis();
        
        // 状態が変わっており、かつデバウンス時間が経過している場合のみ処理
        if (reading != _previousStates[i] && (currentTime - _lastChangeTimes[i] >= _debounceDelay)) {
            _currentStates[i] = reading;
            _lastChangeTimes[i] = currentTime; // 状態変更時間を記録
            if (_callback) {
                _callback(i, _currentStates[i]);  // コールバック呼び出し
            }
            _previousStates[i] = _currentStates[i];
        }
    }
}

// コールバック関数の設定
void PedalMonitor::setCallback(void (*callback)(int, int)) {
    _callback = callback;
}