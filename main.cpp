#include <iostream>
#include <locale>
using namespace std;

class DynamicArray {
private:
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
        if (index < 0 || index >= size) {
            cout << "error" << endl;
            return;
        }
        if (value < -100 || value > 100) {
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
};

int main() {
    cout << "=== Task 1 ===" << endl;
    DynamicArray arr1(3);
    arr1.set(0, 10);
    arr1.set(1, -20);
    arr1.set(2, 30);
    arr1.print();

    cout << "=== Task 2 ===" << endl;
    DynamicArray arr2 = arr1;
    arr2.print();

    cout << "=== Task 3 ===" << endl;
    arr1.push_back(50);
    arr1.print();

    cout << "=== Task 4 ===" << endl;
    DynamicArray arr3(2);
    arr3.set(0, 5);
    arr3.set(1, 15);

    cout << "arr1: "; arr1.print();
    cout << "arr3: "; arr3.print();

    DynamicArray sum = arr1.add(arr3);
    cout << "Sum: "; sum.print();

    DynamicArray diff = arr1.sub(arr3);
    cout << "Difference: "; diff.print();

    return 0;
}
