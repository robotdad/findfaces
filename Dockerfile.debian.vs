FROM findfaces/build

LABEL description="Container for use with VS"

RUN apt-get update && apt-get install -y --no-install-recommends \
    gdb \
    gdbserver \
    openssh-server \
    rsync \
    zip \
    --fix-missing \
    && rm -rf /var/lib/apt/lists/*

RUN mkdir /var/run/sshd && \
    echo 'root:toor' | chpasswd && \
	echo 'PermitRootLogin yes' >> /etc/ssh/sshd_config && \
	sed 's@session\s*required\s*pam_loginuid.so@session optional pam_loginuid.so@g' -i /etc/pam.d/sshd

RUN mkdir /usr/local/files
COPY /src/haarcascade_frontalface_alt2.xml /usr/local/files/haarcascade_frontalface_alt2.xml

EXPOSE 22
CMD ["/usr/sbin/sshd", "-D"]
