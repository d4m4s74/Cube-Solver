# 1. Use the official lightweight Python image
FROM python:3.11-slim

# 2. Install 'make' and 'gcc' compiler tools required for your C library
RUN apt-get update && apt-get install -y \
    make \
    gcc \
    libc-dev \
    && rm -rf /var/lib/apt/lists/*

# 3. Set the working directory inside the container
WORKDIR /app

# 4. Copy the requirements file first
COPY requirements.txt .

# 5. Install the Python dependencies
RUN pip install --no-cache-dir -r requirements.txt

# 6. Copy the rest of your application code (including the src/ folder and Makefile)
COPY . .

# 7. Create the output directory if it doesn't exist, then compile the library
RUN mkdir -p bin && make library

# 8. Set environment variables needed for Flask
ENV FLASK_APP=app.py
ENV FLASK_RUN_HOST=0.0.0.0

# 9. Expose the default Flask port
EXPOSE 5000

# 10. The startup command to run the application
CMD ["flask", "run"]
