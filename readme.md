# CodeCounter 🚀

![C++](https://img.shields.io/badge/C++-23-blue.svg)
![License](https://img.shields.io/badge/License-MIT-green.svg)

**CodeCounter** is aiming to be blazing-fast, modern C++ Command Line Interface (CLI) utility designed for analyzing and counting lines of code, comments, and blank lines across large-scale codebases. 

Built with **systems programming** principles in mind, it leverages C++23 features to deliver maximum performance through zero-cost abstractions, memory safety, and highly optimized I/O operations.

Please note that this tool is under active development.

---

## ✨ Features

*   **Zero-Cost String Parsing:** Utilizes `std::string_view` extensively to eliminate unnecessary memory allocations and copying during file and configuration parsing, strictly preventing dangling references.
*   **Extensible Architecture (SOLID):** Implements the **Strategy Pattern** for report generation. Output logic is decoupled using a pure virtual `IReportFormatter` interface and `std::ostream` polymorphism, allowing seamless additions of new formats without modifying core logic.
*   **Multiple Output Formats:** Generate code statistics in beautifully formatted CLI Tables (with custom ANSI colors), JSON, or Markdown files.
*   **Robust CLI Parser:** A defensive and strict command-line argument parser that handles missing arguments, invalid paths, and directory/file distinctions securely.
*   **Modern C++ Standards:** Makes heavy use of C++17/20/23 features including `std::filesystem`, `std::println`, `std::unique_ptr` (RAII), and structured bindings.

---

## 🏗️ Architecture

CodeCounter is built on **Clean Architecture** principles. The core business logic (analyzing files) is strictly separated from the presentation layer (formatting output).

### The Reporting Strategy
Instead of monolithic `if/else` blocks, the tool dynamically injects the requested formatting strategy at runtime:

```cpp
// Interface
class IReportFormatter {
public:
    virtual ~IReportFormatter() = default;
    virtual void format(std::ostream& os, const std::map<std::string, types::FileStats>& stats) const = 0;
};

```

Depending on the user's CLI arguments (`--json`, `--m`, or default table), the corresponding formatter (`JsonFormatter`, `MarkdownFormatter`, or `TableFormatter`) is instantiated and passed to either `std::cout` or a `std::ofstream`.

---

## 🚀 Roadmap / Upcoming Features

We are constantly pushing the boundaries of performance. The following features will be implemented in near time:

* **Multithreading & Concurrency:** Distributing file I/O and parsing workloads across multiple CPU cores using `std::thread`, `std::mutex`, and `std::atomic` to prevent race conditions.
* **Memory-Mapped Files (mmap):** Bypassing standard `std::ifstream` overhead by mapping files directly into RAM and utilizing `reinterpret_cast` for raw byte processing, aiming to parse millions of lines in milliseconds.

---

## 🛠️ Build Instructions

### Prerequisites

* A C++23 compatible compiler (GCC 13+, Clang 16+, or MSVC 19.38+)

### Information

This readme will be updated when MVP is released.

