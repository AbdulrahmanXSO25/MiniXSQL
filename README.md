# MINIXSQL

MINIXSQL is a lightweight, educational, relational database management system written in C.

## Features

- In-memory data storage (*currently*, I plan to implement disk IO)
- Basic CRUD operations
- Support for multiple data types (INT, VARCHAR, TIMESTAMP, etc.)
- Simple table creation and management

### Prerequisites

- GCC compiler
- Make build system
- Linux or macOS operating system

### Building the Project

1. Clone the repository:
```bash
git clone https://github.com/abdulrahmanxso25/minixsql.git
```

2. Build the project:
```bash
make
```

3. Run the program:
```bash
make run
```

## Usage

The current version demonstrates basic functionality by creating sample tables (employees, departments, projects), inserting data, and printing the results.

## Contributing

Contributions are more than welcome! Please feel free to submit a Pull Request.

## License

This project is licensed under the GNU General Public License v3.0 - see the [LICENSE](LICENSE) file for details.

## Acknowledgments

- Inspired by PostgreSQL