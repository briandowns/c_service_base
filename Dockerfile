FROM ubuntu:latest

RUN apt-get update &&   \
    apt-get install -y  \
        libjson-c-dev   \
        libjansson-dev  \
        libyder-dev     \
        liborcania-dev  \
        libulfius-dev   \
        jq              \
        make            \
        build-essential \
        git

COPY . /code
RUN cd /code && \
    make &&     \
    cp bin/simple-svc /simple-svc
    
EXPOSE 8080:8080
CMD [ "/simple-svc" ]
