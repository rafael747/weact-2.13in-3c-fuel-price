# weact-2.13in-3c-fuel-prices

ESPHome Fuel Prices for the WeAct 2.13" 3 colors e-paper display (ESP32-S3).

## Requirements

- [Docker](https://docs.docker.com/get-docker/) + Docker Compose
- No host Python / ESPHome / PlatformIO install needed

## Setup

```bash
cp secrets.yaml.example secrets.yaml
# Edit secrets.yaml: wifi_ssid, wifi_password, latitude, longitude, apikey
```

## Dashboard

```bash
docker compose up -d
open http://localhost:6052
```

Stop with `docker compose down`.

## CLI (one-shot)

```bash
# Validate config
docker compose run --rm esphome config weact-2.13in-3c.yaml

# Compile
docker compose run --rm esphome compile weact-2.13in-3c.yaml

# Upload over Wi‑Fi (OTA) — preferred on macOS
docker compose run --rm esphome upload weact-2.13in-3c.yaml

# Logs
docker compose run --rm esphome logs weact-2.13in-3c.yaml
```

## Flashing on macOS

Docker Desktop cannot pass USB serial devices through reliably.

| Situation | Approach |
|---|---|
| Device already on Wi‑Fi | OTA via dashboard or `upload` |
| First flash / recovery | Dashboard → Install → **Plug into this computer** (Chrome/Edge Web Serial), or flash with `esptool` on the host |

## Notes

- PlatformIO / toolchain cache is bind-mounted at `./.cache` → `/cache` (persists across `docker compose run --rm`).
- Build trees live in `./.esphome/` via the project bind mount (`.:/config`). On macOS/Colima this can be slower than a named volume because of lots of small files across the VM filesystem boundary; named volumes are faster if compiles still feel slow.
- Device online status uses ping (`ESPHOME_DASHBOARD_USE_PING`) because host networking is unavailable on Docker Desktop Mac.
- If compile fails with `Killed signal terminated program`, the container ran out of RAM. `compile_process_limit: 1` is already set in the YAML; also give Docker Desktop **≥ 8 GB** memory (Settings → Resources).

## References

- https://creativecommons.tankerkoenig.de/swagger
- https://github.com/rafael747/lilygo-t5-weather-station