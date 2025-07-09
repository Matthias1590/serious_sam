FROM debian:bookworm-slim AS downloader

RUN mkdir /app
WORKDIR /app

RUN apt-get update && apt-get install -y \
	wget \
	p7zip-full \
	&& rm -rf /var/lib/apt/lists/*

RUN wget -O serious_sam.7z https://archive.org/download/serious-sam-classic-serious-sam-classic-vk-v-1.10.6d-linux-amd-64-gog-assets/Serious_Sam_Classic_%28SeriousSamClassic-VK%2C_v1.10.6d%29_%5BLinux%2C_amd64%2C_GOG-assets%5D.7z
RUN 7z x serious_sam.7z
RUN rm -f serious_sam.7z
RUN 7z x "game-Serious_Sam_Classic_(SeriousSamClassic-VK,_v1.10.6d)_[Linux,_amd64,_GOG-assets].7z"
RUN rm -f *.rar *.7z *.jpg

FROM debian:bookworm-slim AS server

RUN mkdir /app
WORKDIR /app

COPY --from=downloader /app/SamTFE /app/SamTFE

RUN apt-get update && apt-get install -y \
	libsdl2-dev \
	libvulkan1

WORKDIR /app/SamTFE

COPY server.ini /app/SamTFE/Scripts/Dedicated/DefaultFragmatch/init.ini

CMD ["./Bin/DedicatedServer", "DefaultFragmatch"]
