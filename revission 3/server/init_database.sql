DO $$
BEGIN
    IF NOT EXISTS (SELECT FROM information_schema.tables WHERE table_name = 'users') THEN
        CREATE TABLE users (
            id SERIAL PRIMARY KEY,
            username VARCHAR(50) UNIQUE NOT NULL,
            password_hash VARCHAR(255) NOT NULL
        );
        RAISE NOTICE 'Table users created';
    ELSE
        RAISE NOTICE 'Table users already exists';
    END IF;
END $$;

DO $$
BEGIN
    IF NOT EXISTS (SELECT FROM information_schema.tables WHERE table_name = 'charts') THEN
        CREATE TABLE charts (
            id SERIAL PRIMARY KEY,
            user_id INTEGER REFERENCES users(id),
            chart_data JSONB,
            created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
        );
        RAISE NOTICE 'Table charts created';
    ELSE
        RAISE NOTICE 'Table charts already exists';
    END IF;
END $$;
DO $$
BEGIN
    IF NOT EXISTS (SELECT FROM information_schema.tables WHERE table_name = 'users') THEN
        CREATE TABLE users (
            id SERIAL PRIMARY KEY,
            username VARCHAR(50) UNIQUE NOT NULL,
            password_hash VARCHAR(255) NOT NULL,
            email VARCHAR(100) NOT NULL
        );
        RAISE NOTICE 'Table users created';
    ELSE
        RAISE NOTICE 'Table users already exists';
    END IF;
END $$;
