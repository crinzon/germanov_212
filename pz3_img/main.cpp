#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include <dirent.h>
#include <random>

#ifdef _WIN32
#include <direct.h>
#else
#include <sys/stat.h>
#endif

class PGMImage {
public:
    int width, height, maxVal;
    std::vector<std::vector<int>> pixels;

    PGMImage() : width(0), height(0), maxVal(0) {}

    bool read(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) return false;

        std::string line;
        std::getline(file, line);
        if (line != "P2") return false;

        do { std::getline(file, line); } while (!line.empty() && line[0] == '#');
        std::istringstream(line) >> width >> height;
        if (width <= 0 || height <= 0) return false;

        do { std::getline(file, line); } while (!line.empty() && line[0] == '#');
        maxVal = std::stoi(line);

        pixels.resize(height, std::vector<int>(width));
        for (int y = 0; y < height; ++y)
            for (int x = 0; x < width; ++x)
                file >> pixels[y][x];

        return true;
    }

    bool writeASCII(const std::string& filename) {
        std::ofstream file(filename);
        if (!file) return false;

        file << "P2\n" << width << " " << height << "\n" << maxVal << "\n";
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x)
                file << pixels[y][x] << " ";
            file << "\n";
        }
        return true;
    }

    void addSaltPepperNoise(double amount) {
        std::mt19937 gen(std::random_device{}());
        std::uniform_real_distribution<> chance(0.0, 1.0);

        for (int y = 0; y < height; ++y)
            for (int x = 0; x < width; ++x) {
                double r = chance(gen);
                if (r < amount / 2.0) pixels[y][x] = 0;
                else if (r < amount) pixels[y][x] = maxVal;
            }
    }

    void medianFilter(int size = 3) {
        int offset = size / 2;
        std::vector<std::vector<int>> temp = pixels;

        for (int y = 0; y < height; ++y)
            for (int x = 0; x < width; ++x) {
                std::vector<int> neighbors;
                for (int dy = -offset; dy <= offset; ++dy)
                    for (int dx = -offset; dx <= offset; ++dx) {
                        int ny = y + dy, nx = x + dx;
                        if (ny >= 0 && ny < height && nx >= 0 && nx < width)
                            neighbors.push_back(temp[ny][nx]);
                    }
                std::sort(neighbors.begin(), neighbors.end());
                pixels[y][x] = neighbors[neighbors.size()/2];
            }
    }

    void movingAverage(int window = 5) {
        std::vector<std::vector<int>> temp = pixels;
        for (int y = 0; y < height; ++y)
            for (int x = 0; x < width; ++x) {
                int sum = 0, count = 0;
                for (int dx = -window/2; dx <= window/2; ++dx) {
                    int nx = x + dx;
                    if (nx >= 0 && nx < width) { sum += temp[y][nx]; count++; }
                }
                pixels[y][x] = sum / count;
            }
    }

    void exponentialMovingAverage(double k = 0.5) {
        for (int y = 0; y < height; ++y) {
            double prev = pixels[y][0];
            for (int x = 1; x < width; ++x) {
                prev = prev + (pixels[y][x] - prev) * k;
                pixels[y][x] = static_cast<int>(prev);
            }
        }
    }

    double compareWithOriginal(const PGMImage& orig) const {
        int diff = 0;
        for (int y = 0; y < height; ++y)
            for (int x = 0; x < width; ++x)
                if (abs(pixels[y][x] - orig.pixels[y][x]) > 1) diff++;
        return 100.0 * diff / (width * height);
    }
};

void createDir(const std::string& path) {
#ifdef _WIN32
    _mkdir(path.c_str());
#else
    mkdir(path.c_str(), 0777);
#endif
}

int main() {
    const std::string inputDir = "image_orig";
    const std::string noiseDir = "image_noise";
    const std::string fixDir = "image_fix";
    const std::vector<int> noiseLevels = {5, 10, 20};

    createDir(noiseDir);
    createDir(fixDir);

    DIR* dir = opendir(inputDir.c_str());
    if (!dir) return 1;

    std::ofstream csv("results.csv");
    csv << "Image;NoisePercent;Filter;InaccuracyPercent\n";

    dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        std::string filename = entry->d_name;
        if (filename.length() < 5 || filename.substr(filename.length() - 4) != ".pgm") continue;

        std::string stem = filename.substr(0, filename.length() - 4);
        std::string path = inputDir + "/" + filename;

        PGMImage orig;
        if (!orig.read(path)) continue;

        for (int n : noiseLevels) {
            PGMImage noisy = orig;
            noisy.addSaltPepperNoise(n / 100.0);
            noisy.writeASCII(noiseDir + "/" + stem + "_" + std::to_string(n) + ".pgm");

            PGMImage medianImg = noisy;
            medianImg.medianFilter();
            medianImg.writeASCII(fixDir + "/" + stem + "_" + std::to_string(n) + "_median.pgm");
            csv << filename << ";" << n << ";median;" << medianImg.compareWithOriginal(orig) << "\n";

            PGMImage movAvgImg = noisy;
            movAvgImg.movingAverage();
            movAvgImg.writeASCII(fixDir + "/" + stem + "_" + std::to_string(n) + "_movavg.pgm");
            csv << filename << ";" << n << ";movingAverage;" << movAvgImg.compareWithOriginal(orig) << "\n";

            PGMImage expAvgImg = noisy;
            expAvgImg.exponentialMovingAverage();
            expAvgImg.writeASCII(fixDir + "/" + stem + "_" + std::to_string(n) + "_expavg.pgm");
            csv << filename << ";" << n << ";expMovingAverage;" << expAvgImg.compareWithOriginal(orig) << "\n";
        }
    }

    closedir(dir);
    csv.close();
    return 0;
}
