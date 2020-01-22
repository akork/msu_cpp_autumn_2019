#include <algorithm>
#include <array>
#include <ctime>
#include <fstream>
#include <iostream>
#include <vector>

#include "../08/tp.h"

#define CHUNK_SIZE 4000000

int comparator(const void *a, const void *b)
{
    return *((uint64_t *)a) - *((uint64_t *)b);
}

void sort_chunk(std::string input_file, int i, int array_size)
{
    std::ifstream fin;
    std::ofstream fout;
    fin.open(input_file, std::ios::binary);
    fout.open("chunk" + std::to_string(i));

    uint64_t *arr = new uint64_t[CHUNK_SIZE];
    fin.seekg(i * CHUNK_SIZE * sizeof(uint64_t));
    if ((fin.rdstate() & std::ifstream::failbit) != 0) {
        std::cout << "error\n";
        return;
    }
    fin.read((char *)arr, CHUNK_SIZE * sizeof(uint64_t));

    int length = fin.gcount() / sizeof(uint64_t);
    qsort(arr, length, sizeof(uint64_t), comparator);

    fout.write((char *)arr, length * sizeof(uint64_t));

    fin.close();
    fout.close();
    delete[] arr;
}

void random_file(int size, std::string file_name)
{
    srand(293);
    uint64_t *a = new uint64_t[size];
    for (int i = 0; i < size; ++i) {
        a[i] = rand() % 1000;
    }
    std::ofstream f;
    f.open(file_name, std::ios::out);
    f.write((char *)a, size * sizeof(uint64_t));
    f.close();
    delete[] a;
}

int main(int argc, char **argv)
{
    assert(argc == 3);
    std::string input_file(argv[1]);
    std::string output_file(argv[2]);

    // random_file(200000, input_file);

    std::ifstream in(input_file, std::ifstream::ate | std::ifstream::binary);
    int file_size = in.tellg();
    in.close();
    int array_size = file_size / sizeof(uint64_t);
    assert(array_size != 0);
    int n_chunks = (array_size + CHUNK_SIZE - 1) / CHUNK_SIZE;
    int n_threads = 2;

    // sort chunks
    {
        ThreadPool pool(n_threads);
        for (int i = 0; i < n_chunks; ++i) {
            pool.exec(sort_chunk, input_file, i, array_size);
        }
    }

    std::ofstream fout;
    int buf_size = CHUNK_SIZE / n_chunks;
    if (buf_size == 0)
        buf_size = 1;
    fout.open(output_file);
    std::vector<std::ifstream> chunk_files;
    std::vector<int> chunk_pos(n_chunks, 0);
    std::vector<int> buf_pos(n_chunks, 0);
    std::vector<uint64_t *> bufs;
    for (int i = 0; i < n_chunks; ++i) {
        chunk_files.emplace_back("chunk" + std::to_string(i), std::ios::binary);
        bufs.push_back(new uint64_t[buf_size]);
        for (int j = 0; j < buf_size; j++) {
            bufs[i][j] = std::numeric_limits<uint64_t>().max();
        }
        chunk_files[i].read((char *)bufs[i], buf_size * sizeof(uint64_t));
    }

    // merge
    for (int j = 0; j < array_size; j++) {
        int argmin = 0;
        uint64_t min;

        min = std::numeric_limits<uint64_t>().max();
        for (int i = 0; i < n_chunks; ++i) {
            if (bufs[i][buf_pos[i]] <= min) {
                min = bufs[i][buf_pos[i]];
                argmin = i;
            }
        }
        buf_pos[argmin]++;
        chunk_pos[argmin]++;
        if (buf_pos[argmin] == buf_size) {
            buf_pos[argmin] = 0;
            for (int i = 0; i < buf_size; ++i)
                bufs[argmin][i] = std::numeric_limits<uint64_t>().max();
            chunk_files[argmin].read((char *)(bufs[argmin]),
                                     buf_size * sizeof(uint64_t));
        }
        // if (j % 10000 == 0) std::cout << j << "::" << min << std::endl;
        fout.write((char *)&min, sizeof(uint64_t));
    }
    for (auto &&buf : bufs)
        delete[] buf;
    for (auto &&chunk_file : chunk_files)
        chunk_file.close();

    std::cout << std::endl;
}
