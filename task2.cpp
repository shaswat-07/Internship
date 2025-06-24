#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>
#include <sstream>
#include <stdexcept>
#include <cstring>
using namespace std;

const size_t CHUNK_SIZE = 1 << 20; // 1 MB
mutex outputMutex;

// Run-Length Encoding compression for a chunk
string rle_compress(const string& data) {
    stringstream ss;
    int n = data.size();
    for (int i = 0; i < n; ) {
        char current = data[i];
        int count = 1;
        while (i + count < n && data[i + count] == current && count < 255)
            count++;
        ss << current << (char)count;
        i += count;
    }
    return ss.str();
}

// Decompression of an RLE-compressed chunk
string rle_decompress(const string& data) {
    stringstream ss;
    int n = data.size();
    for (int i = 0; i < n; i += 2) {
        if (i + 1 >= n) throw runtime_error("Corrupted compressed data.");
        char ch = data[i];
        int count = (unsigned char)data[i + 1];
        ss << string(count, ch);
    }
    return ss.str();
}

// Compress a chunk in a thread and store it in the result vector
void compress_chunk(const string& chunk, vector<string>& results, int index) {
    string compressed = rle_compress(chunk);
    lock_guard<mutex> lock(outputMutex);
    results[index] = compressed;
}

// Decompress a chunk in a thread
void decompress_chunk(const string& chunk, vector<string>& results, int index) {
    string decompressed = rle_decompress(chunk);
    lock_guard<mutex> lock(outputMutex);
    results[index] = decompressed;
}

// Read file into chunks
vector<string> read_file_chunks(const string& filepath) {
    ifstream in(filepath, ios::binary);
    if (!in) throw runtime_error("Error opening input file.");
    vector<string> chunks;
    while (!in.eof()) {
        string chunk(CHUNK_SIZE, '\0');
        in.read(&chunk[0], CHUNK_SIZE);
        chunk.resize(in.gcount());
        if (!chunk.empty()) chunks.push_back(chunk);
    }
    return chunks;
}

// Write string chunks to output file
void write_chunks_to_file(const string& filepath, const vector<string>& chunks) {
    ofstream out(filepath, ios::binary);
    if (!out) throw runtime_error("Error opening output file.");
    for (const string& chunk : chunks)
        out.write(chunk.data(), chunk.size());
}

// Compress using multithreading
void compress(const string& input_path, const string& output_path) {
    auto chunks = read_file_chunks(input_path);
    vector<string> results(chunks.size());
    vector<thread> threads;

    auto start = chrono::high_resolution_clock::now();
    for (size_t i = 0; i < chunks.size(); ++i)
        threads.emplace_back(compress_chunk, cref(chunks[i]), ref(results), i);
    for (thread& t : threads) t.join();
    auto end = chrono::high_resolution_clock::now();

    write_chunks_to_file(output_path, results);

    chrono::duration<double> duration = end - start;
    cout << "Multi-threaded compression completed in " << duration.count() << " seconds.\n";
}

// Decompress using multithreading
void decompress(const string& input_path, const string& output_path) {
    auto chunks = read_file_chunks(input_path);
    vector<string> results(chunks.size());
    vector<thread> threads;

    auto start = chrono::high_resolution_clock::now();
    for (size_t i = 0; i < chunks.size(); ++i)
        threads.emplace_back(decompress_chunk, cref(chunks[i]), ref(results), i);
    for (thread& t : threads) t.join();
    auto end = chrono::high_resolution_clock::now();

    write_chunks_to_file(output_path, results);

    chrono::duration<double> duration = end - start;
    cout << "Multi-threaded decompression completed in " << duration.count() << " seconds.\n";
}

// Validate if decompressed file matches original
bool validate(const string& original_path, const string& decompressed_path) {
    ifstream f1(original_path, ios::binary);
    ifstream f2(decompressed_path, ios::binary);
    if (!f1 || !f2) return false;

    string b1, b2;
    while (!f1.eof() || !f2.eof()) {
        char buf1[CHUNK_SIZE], buf2[CHUNK_SIZE];
        f1.read(buf1, CHUNK_SIZE);
        f2.read(buf2, CHUNK_SIZE);
        if (f1.gcount() != f2.gcount() || memcmp(buf1, buf2, f1.gcount()) != 0)
            return false;
    }
    return true;
}

// Benchmark single-threaded vs multithreaded compression
void benchmark(const string& input_path) {
    auto chunks = read_file_chunks(input_path);

    // Single-threaded
    vector<string> single_result(chunks.size());
    auto start_single = chrono::high_resolution_clock::now();
    for (size_t i = 0; i < chunks.size(); ++i)
        single_result[i] = rle_compress(chunks[i]);
    auto end_single = chrono::high_resolution_clock::now();

    // Multi-threaded
    vector<string> multi_result(chunks.size());
    vector<thread> threads;
    auto start_multi = chrono::high_resolution_clock::now();
    for (size_t i = 0; i < chunks.size(); ++i)
        threads.emplace_back(compress_chunk, cref(chunks[i]), ref(multi_result), i);
    for (thread& t : threads) t.join();
    auto end_multi = chrono::high_resolution_clock::now();

    chrono::duration<double> t_single = end_single - start_single;
    chrono::duration<double> t_multi = end_multi - start_multi;

    cout << "\n Benchmark:\n";
    cout << "Single-threaded time: " << t_single.count() << " sec\n";
    cout << "Multi-threaded time: " << t_multi.count() << " sec\n";
    cout << "Speedup: " << t_single.count() / t_multi.count() << "x faster\n";
}

int main() {
    string input = "input.txt";
    string compressed = "compressed.rle";
    string decompressed = "output.txt";

    try {
        // Run compression
        compress(input, compressed);

        // Run decompression
        decompress(compressed, decompressed);

        // Validate result
        if (validate(input, decompressed))
            cout << "Validation: Decompressed file matches original.\n";
        else
            cout << "Validation failed: Files differ!\n";

        // Benchmark performance
        benchmark(input);
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << "\n";
    }

    return 0;
}
