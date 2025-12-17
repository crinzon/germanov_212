#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <ctime>

using namespace std;

class DynamicArray {
protected:
    int* data;
    int size;

    static string currentDateTimeString() {
        auto now = chrono::system_clock::now();
        time_t t = chrono::system_clock::to_time_t(now);

        tm local_tm{};
#ifdef _WIN32
        localtime_s(&local_tm, &t);
#else
        localtime_r(&t, &local_tm);
#endif

        ostringstream oss;
        oss << put_time(&local_tm, "%Y-%m-%d_%H-%M-%S");
        return oss.str();
    }

    static string makeFileName(const string& ext) {
        return currentDateTimeString() + ext;
    }

public:
    DynamicArray(int s) {
        if (s <= 0) s = 1;
        size = s;
        data = new int[size]{};
    }

    virtual ~DynamicArray() {
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

    int getSize() const { return size; }

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

    virtual void save() const {
        cout << "error" << endl;
    }
};

class ArrTxt : public DynamicArray {
public:
    ArrTxt(int s) : DynamicArray(s) {}

    void save() const override {
        string filename = makeFileName(".txt");
        ofstream out(filename);
        if (!out) {
            cout << "error" << endl;
            return;
        }

        for (int i = 0; i < size; ++i) {
            out << data[i] << (i < size - 1 ? " " : "");
        }
        out << "\n";
        out.close();

        cout << "Saved to: " << filename << endl;
    }
};

class ArrCSV : public DynamicArray {
public:
    ArrCSV(int s) : DynamicArray(s) {}

    void save() const override {
        string filename = makeFileName(".csv");
        ofstream out(filename);
        if (!out) {
            cout << "error" << endl;
            return;
        }

        for (int i = 0; i < size; ++i) {
            out << data[i];
            if (i < size - 1) out << ",";
        }
        out << "\n";
        out.close();

        cout << "Saved to: " << filename << endl;
    }
};

void saveArray(DynamicArray* arr) {
    arr->save();
}

int main() {
    DynamicArray* arrays[2];
    arrays[0] = new ArrTxt(3);
    arrays[1] = new ArrCSV(3);

    arrays[0]->set(0, 10);
    arrays[0]->set(1, -20);
    arrays[0]->set(2, 30);

    arrays[1]->set(0, 5);
    arrays[1]->set(1, 15);
    arrays[1]->set(2, -7);

    for (int i = 0; i < 2; i++) {
        arrays[i]->print();
        saveArray(arrays[i]);
        delete arrays[i];
    }

    return 0;
}
