# Contributing to cconfig

First off, thank you for considering contributing to `cconfig`! It's people like you that make open source such a great community. We welcome any and all contributions.

## Where do I start?

Not sure where to begin? You can start by looking through these `good first issue` and `help wanted` issues:

*   **Good first issues** - issues which should only require a few lines of code, and a test or two.
*   **Help wanted issues** - issues which should be a bit more involved than `good first issue` issues.

If you have a new idea, feel free to open a Feature Request issue to discuss it.

## Contribution Workflow

Please follow this workflow to contribute:

1.  **Fork the repository** to your own GitHub account.
2.  **Clone your fork** to your local machine:
    ```bash
    git clone https://github.com/YOUR_USERNAME/cconfig.git
    cd cconfig
    ```
3.  **Create a new branch** for your changes. Choose a descriptive name.
    ```bash
    git checkout -b your-branch-name
    ```
4.  **Make your changes**. Since `cconfig` is a single-header library, you will likely be editing `cconfig.h`.

5.  **Test your changes**. `cconfig` has a test suite. Please add tests for any new functionality or bug fixes. You can run the tests with the following commands:

    **On Linux/macOS:**
    ```bash
    gcc -o tests/test_runner tests/test_runner.c -I. -std=c99 -Wall -Wextra -pedantic
    ./tests/test_runner
    ```

    **On Windows (with MinGW/MSYS2):**
    ```bash
    gcc -o tests/test_runner.exe tests/test_runner.c -I. -std=c99 -Wall -Wextra -pedantic -mconsole
    ./tests/test_runner.exe
    ```

    Ensure all tests pass before submitting your changes.

6.  **Commit your changes**. Use a clear and descriptive commit message.
    ```bash
    git add .
    git commit -m "feat: Add this new feature"
    ```
    (We recommend following the Conventional Commits specification.)

7.  **Push your branch** to your fork on GitHub:
    ```bash
    git push origin your-branch-name
    ```
8.  **Open a Pull Request** from your branch to the `main` branch of the `Nouridin/cconfig` repository. Provide a clear description of the changes you've made.

## Code Style

*   This is a single-header library. All implementation code goes inside the `#ifdef CCONFIG_IMPLEMENTATION` block in `cconfig.h`.
*   Please try to match the existing code style (variable naming, bracing, etc.).
*   Use C99 standard features.
*   Add comments to new functions or complex logic.

## Code of Conduct

This project and everyone participating in it is governed by the cconfig Code of Conduct. By participating, you are expected to uphold this code.

Thank you again for your contribution!