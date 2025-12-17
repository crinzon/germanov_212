#include <iostream>
#include <stdexcept>

using namespace std;

class DynamicArray {
private:
    int* data;
    int size;

    void checkIndex(int index) const {
        if (index < 0 || index >= size)
            throw out_of_range("out_of_range");
    }

    void checkValue(int value) const {
        if (value < -100 || value > 100)
            throw invalid_argument("invalid_argument");
    }

public:
    DynamicArray(int s) {
        if (s <= 0) s = 1;
        size = s;
        data = new int[size]{};
    }

    ~DynamicArray() {
        delete[] data;
    }

    DynamicArray(const DynamicArray& other) {
        size = other.size;
        data = new int[size];
        for (int i = 0; i < size; ++i)
            data[i] = other.data[i];
    }

    DynamicArray& operator=(const DynamicArray& other) {
        if (this == &other) return *this;
        delete[] data;
        size = other.size;
        data = new int[size];
        for (int i = 0; i < size; ++i)
            data[i] = other.data[i];
        return *this;
    }

    void set(int index, int value) {
        checkIndex(index);
        checkValue(value);
        data[index] = value;
    }

    int get(int index) const {
        checkIndex(index);
        return data[index];
    }

    void print() const {
        cout << "{ ";
        for (int i = 0; i < size; ++i)
            cout << data[i] << (i < size - 1 ? ", " : " ");
        cout << "}" << endl;
    }

    void push_back(int value) {
        checkValue(value);
        int* newData = new int[size + 1];
        for (int i = 0; i < size; ++i)
            newData[i] = data[i];
        newData[size] = value;
        delete[] data;
        data = newData;
        size++;
    }

    DynamicArray add(const DynamicArray& other) const {
        int maxSize = (size > other.size) ? size : other.size;
        DynamicArray result(maxSize);
        for (int i = 0; i < maxSize; ++i) {
            int a = (i < size) ? data[i] : 0;
            int b = (i < other.size) ? other.data[i] : 0;
            result.data[i] = a + b;
        }
        return result;
    }

    DynamicArray sub(const DynamicArray& other) const {
        int maxSize = (size > other.size) ? size : other.size;
        DynamicArray result(maxSize);
        for (int i = 0; i < maxSize; ++i) {
            int a = (i < size) ? data[i] : 0;
            int b = (i < other.size) ? other.data[i] : 0;
            result.data[i] = a - b;
        }
        return result;
    }
};

int main() {
    try {
        DynamicArray arr(3);
        arr.set(0, 10);
        arr.set(1, -20);
        arr.set(2, 30);
        arr.print();

        arr.push_back(50);
        arr.print();

    } catch (const exception& e) {
        cout << e.what() << endl;
    }
    return 0;
}
