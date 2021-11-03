# See here for image contents: https://github.com/microsoft/vscode-dev-containers/tree/v0.202.5/containers/debian/.devcontainer/base.Dockerfile

FROM mcr.microsoft.com/vscode/devcontainers/base:0-bullseye AS base

RUN set -x \
    && export DEBIAN_FRONTEND=noninteractive \
    && apt-get -y update \
    && apt-get -y install --no-install-recommends \
        doxygen \
        graphviz \
        python3 \
        python3-pip \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /workspaces/btree
COPY configure .
RUN set -x \
    && ./configure \
    && rm -rf /workspaces/btree

FROM base AS fuzz
WORKDIR /workspaces/btree
COPY . .
RUN set -x \
    && meson build \
