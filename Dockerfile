dockerfile
FROM ubuntu:22.04 AS builder
RUN apt-get update && apt-get install -y cmake g++ && rm -rf
/var/lib/apt/lists/*
WORKDIR /app
COPY . .
RUN cmake -B build -DCMAKE_BUILD_TYPE=Release && cmake --
build build
FROM ubuntu:22.04
WORKDIR /app
COPY --from=builder /app/build/matrix_calculator .
CMD ["./matrix_calculator"]
