# ---------- BUILD STAGE ----------
FROM debian:bullseye AS builder

RUN apt-get update && apt-get install -y \
    build-essential \
    qtbase5-dev \
    qt5-qmake \
    qtbase5-dev-tools \
    libqt5sql5-psql \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY . .

RUN qmake serverdb.pro && make


# ---------- RUNTIME STAGE ----------
FROM debian:bullseye

RUN apt-get update && apt-get install -y \
    libqt5core5a \
    libqt5network5 \
    libqt5sql5 \
    libqt5sql5-psql \
    postgresql-client \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY --from=builder /app/serverdb .

EXPOSE 33333

CMD bash -c "until pg_isready -h db -p 5432; do echo waiting for db; sleep 2; done; ./serverdb"