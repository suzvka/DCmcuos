class BasePin {
public:
	virtual ~BasePin() = default;
	// 获取引脚编号
	virtual int getPinNumber() const = 0;
};

// 数字输入
class DigitalInputPin : public BasePin {
};

// 模拟输入
class AnalogInputPin : public BasePin {
};

// 数字输出
class DigitalOutputPin : public BasePin {
};