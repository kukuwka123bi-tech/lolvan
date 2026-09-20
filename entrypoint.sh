#!/bin/sh
set -e

PORT="${PORT:-10000}"
echo "Starting sing-box on port ${PORT}..."

# Подставляем порт Render в конфиг
sed -i "s/\"listen_port\": 10000/\"listen_port\": ${PORT}/g" /app/config.json

exec /app/sing-box run -c /app/config.json
