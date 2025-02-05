set -e
echo "Running qmake..."
qmake main.pro

echo "Building the project..."
make

echo "Launching the app..."
    ./main.app/Contents/MacOS/main  # macOS
