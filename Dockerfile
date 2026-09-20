FROM alpine:latest

RUN apk add --no-cache curl tar ca-certificates tzdata

WORKDIR /app

# Установка последней стабильной версии sing-box
RUN curl -fsSL "https://github.com/SagerNet/sing-box/releases/download/v1.11.4/sing-box-1.11.4-linux-amd64.tar.gz" | \
    tar -xz --strip-components=1 -C /app && \
    chmod +x /app/sing-box

COPY config.json /app/config.json
COPY entrypoint.sh /app/entrypoint.sh
RUN chmod +x /app/entrypoint.sh

ENV PORT=10000
EXPOSE $PORT

ENTRYPOINT ["/app/entrypoint.sh"]
