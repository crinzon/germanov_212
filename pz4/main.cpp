#include <iostream>
#include <algorithm> // для sort
using namespace std;

// Базовый класс DynamicArray
class DynamicArray {
protected:
    int* data;
    int size;

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

    void set(int index, int value) {
        if (index < 0 || index >= size || value < -100 || value > 100) {
            cout << "error" << endl;
            return;
        }
        data[index] = value;
    }

    int get(int index) const {
        if (index < 0 || index >= size) {
            cout << "error" << endl;
            return 0;
        }
        return data[index];
    }

    void print() const {
        cout << "{ ";
        for (int i = 0; i < size; ++i)
            cout << data[i] << (i < size - 1 ? ", " : " ");
        cout << "}" << endl;
    }

    void push_back(int value) {
        if (value < -100 || value > 100) {
            cout << "error" << endl;
            return;
        }
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

    int getSize() const {
        return size;
    }
};

// Наследник с дополнительными методами
class ExtendedArray : public DynamicArray {
public:
    ExtendedArray(int s) : DynamicArray(s) {}

    double average() const {
        if (size == 0) return 0;
        int sum = 0;
        for (int i = 0; i < size; ++i)
            sum += data[i];
        return (double)sum / size;
    }

    double median() const {
        if (size == 0) return 0;
        int* temp = new int[size];
        for (int i = 0; i < size; ++i)
            temp[i] = data[i];

        sort(temp, temp + size);
        double med;
        if (size % 2 == 0)
            med = (temp[size / 2 - 1] + temp[size / 2]) / 2.0;
        else
            med = temp[size / 2];

        delete[] temp;
        return med;
    }

    int min() const {
        if (size == 0) return 0;
        int minimum = data[0];
        for (int i = 1; i < size; ++i)
            if (data[i] < minimum) minimum = data[i];
        return minimum;
    }

    int max() const {
        if (size == 0) return 0;
        int maximum = data[0];
        for (int i = 1; i < size; ++i)
            if (data[i] > maximum) maximum = data[i];
        return maximum;
    }
};

// Пример использования
int main() {
    ExtendedArray arr(5);
    arr.set(0, 10);
    arr.set(1, -20);
    arr.set(2, 30);
    arr.set(3, 40);
    arr.set(4, 0);

    cout << "Array: "; arr.print();
    cout << "Average: " << arr.average() << endl;
    cout << "Median: " << arr.median() << endl;
    cout << "Min: " << arr.min() << endl;
    cout << "Max: " << arr.max() << endl;

    return 0;
}
