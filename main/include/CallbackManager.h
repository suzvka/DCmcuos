#pragma once
#include "Config.h"
#include <cstdint>
#include <type_traits>

class BaseCallback {
public:
	virtual ~BaseCallback() = default;
	virtual bool has() const = 0;
};


class Uint8Callback : BaseCallback {
private:
	void(*callback_ptr)(uint8_t);

public:
	Uint8Callback() : callback_ptr(nullptr) {}
	Uint8Callback(void(*func)(uint8_t)) : callback_ptr(func) {}

	Uint8Callback& operator=(void(*func)(uint8_t)) {
		set(func);
		return *this;
	}

	void operator()(uint8_t param) {
		if (callback_ptr) {
			callback_ptr(param);
		}
	}

	void set(void(*func)(uint8_t)) {
		callback_ptr = func;
	}

	bool has() const {
		return callback_ptr != nullptr;
	}

	void(*get_callback_ptr() const)(uint8_t) {
		return callback_ptr;
	}

};

class ProcessCallback : public BaseCallback {
private:
    void(*callback_ptr)();
public:
    ProcessCallback() : callback_ptr(nullptr) {}
    ProcessCallback(void(*func)()) : callback_ptr(func) {}
	

    ProcessCallback& operator=(void(*func)()) {
        set(func);
        return *this;
    }

    void operator()() {
        if (callback_ptr) {
            callback_ptr();
        }
    }

    void set(void(*func)()) {
        callback_ptr = func;
    }

    bool has() const {
        return callback_ptr != nullptr;
    }

    void(*get_callback_ptr() const)() {
        return callback_ptr;
    }
};

class TaskAgrntCallback : public BaseCallback {
private:
	void(*callback_ptr)(void(*)());
public:
	TaskAgrntCallback() : callback_ptr(nullptr) {}
	TaskAgrntCallback(void(*func)(void(*)())) : callback_ptr(func) {}

	TaskAgrntCallback& operator=(void(*func)(void(*)())) {
		set(func);
		return *this;
	}

	void operator()(void(*task)()) {
		if (callback_ptr) {
			callback_ptr(task);
		}
	}

	void set(void(*func)(void(*)())) {
		callback_ptr = func;
	}

	bool has() const {
		return callback_ptr != nullptr;
	}

	void(*get_callback_ptr() const)(void(*)()) {
		return callback_ptr;
	}
};