FROM python:3-slim
LABEL org.opencontainers.image.source=https://github.com/keyonjerome/linearbookscanner

RUN apt-get update \
    && DEBIAN_FRONTEND=noninteractive apt-get upgrade -y \
    && pip3 install --upgrade pip \
    && apt-get clean

# Create a new user
RUN adduser --quiet --disabled-password --shell /bin/sh --home /home/dockeruser --gecos "" --uid 1000 dockeruser
USER dockeruser
ENV HOME /home/dockeruser
ENV PYTHONPATH "${PYTHONPATH}:/home/dockeruser/.local/bin"
ENV PATH="/home/dockeruser/.local/bin:${PATH}"

# The 'SOURCE' argument is what will be used in 'pip install'.
ARG SOURCE

# Set this argument if running the pip install on a local directory, so
# the local dist files are copied into the container.
ARG DIST_PATH

USER root
RUN mkdir -p /worker && chown dockeruser /worker
USER dockeruser
WORKDIR /worker

COPY --chown=dockeruser $DIST_PATH $DIST_PATH
USER dockeruser
RUN pip3 install --no-cache-dir --force --user --index-url https://pypi.org/simple/ --extra-index-url https://test.pypi.org/simple/ $SOURCE \
    && rm -rf $DIST_PATH

COPY --chown=dockeruser ./docker/docker-entrypoint.sh docker-entrypoint.sh
# Run the subsetter
ENTRYPOINT ["./docker-entrypoint.sh"]