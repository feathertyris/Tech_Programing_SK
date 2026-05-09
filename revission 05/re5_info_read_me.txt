-- Создание пользователя с новым именем и паролем
CREATE USER timp_user WITH PASSWORD 'timp123';

-- Создание базы данных с новым именем, назначаем владельца
CREATE DATABASE timp_database OWNER timp_user;

-- Предоставление всех привилегий на базу данных новому пользователю
GRANT ALL PRIVILEGES ON DATABASE timp_database TO timp_user;

psql -h localhost -U timp_user -d timp_database -p 5432

SELECT id, login, email, created_at
FROM users
ORDER BY created_at;


ALTER TABLE users ADD COLUMN IF NOT EXISTS reset_token VARCHAR(100);
ALTER TABLE users ADD COLUMN IF NOT EXISTS reset_token_expires TIMESTAMP;

SELECT * FROM users*; -просмотр данных в таблице
