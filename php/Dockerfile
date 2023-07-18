FROM php:7.4-fpm

RUN apt-get update -y
RUN apt-get -y install gcc make autoconf libc-dev pkg-config libzip-dev

RUN apt-get install -y --no-install-recommends --assume-yes \
	git \
	libmemcached-dev \
	libz-dev \
	libpq-dev \
	libssl-dev libssl-doc libsasl2-dev \
	libmcrypt-dev \
	libxml2-dev \
	zlib1g-dev libicu-dev g++ \
	libldap2-dev libbz2-dev \
	curl libcurl4-openssl-dev \
	libgmp-dev firebird-dev libib-util \
	re2c libpng++-dev \
	libwebp-dev libjpeg-dev libjpeg62-turbo-dev libpng-dev libxpm-dev libvpx-dev libfreetype6-dev \
	libmagick++-dev \
	libmagickwand-dev \
	zlib1g-dev libgd-dev \
	libtidy-dev libxslt1-dev libmagic-dev libexif-dev file \
	sqlite3 libsqlite3-dev libxslt-dev \
	libmhash2 libmhash-dev libc-client-dev libkrb5-dev libssh2-1-dev \
	unzip libpcre3 libpcre3-dev \
	poppler-utils ghostscript libmagickwand-6.q16-dev libsnmp-dev libedit-dev libreadline6-dev libsodium-dev \
	freetds-bin freetds-dev freetds-common libct4 libsybdb5 tdsodbc libreadline-dev librecode-dev libpspell-dev libonig-dev \
	ca-certificates vim

# RUN docker-php-ext-configure imap --with-kerberos --with-imap-ssl && \
# 	docker-php-ext-install cli mcrypt gd mysql pdo pdo_mysql imap pgsql pdo_pgsql curl bcmath sqlite3 pdo_sqlite3
RUN docker-php-ext-configure gd --with-freetype --with-jpeg
RUN docker-php-ext-install pdo pdo_mysql bcmath soap gd

# install pecl extension
RUN pecl install igbinary-3.2.0 memcached redis mcrypt xdebug-3.1.6 ssh2 && \
	docker-php-ext-enable igbinary redis memcached mcrypt xdebug ssh2

RUN pecl install apcu imagick \
    && pecl install apcu_bc-1.0.5 \
    && docker-php-ext-enable apcu --ini-name 10-docker-php-ext-apcu.ini \
    && docker-php-ext-enable imagick apc --ini-name 20-docker-php-ext-apc.ini 

# RUN sed -i -e 's:<!-- <policy domain="module" rights="none" pattern="{PS,PDF,XPS}" /> -->:<policy domain="module" rights="read|write" pattern="{PS,PDF,XPS}" />:g' '/etc/ImageMagick-6/policy.xml'
# RUN sed -i -e 's:<policy domain="coder" rights="none" pattern="PDF" />:<!--<policy domain="coder" rights="none" pattern="PDF" /> -->:g' '/etc/ImageMagick-6/policy.xml'

RUN sed -i '/disable ghostscript format types/,+6d' /etc/ImageMagick-6/policy.xml

RUN curl -Lsf 'https://golang.org/dl/go1.16.6.linux-amd64.tar.gz' | tar -C '/usr/local' -xvzf -
ENV PATH /usr/local/go/bin:$PATH

RUN go get github.com/mailhog/mhsendmail \
 	&& cp /root/go/bin/mhsendmail /usr/bin/mhsendmail \
	&& echo 'sendmail_path="/usr/bin/mhsendmail --smtp-addr=10.204.2.100:1025 --from=noreply@mailhog.local"' > /usr/local/etc/php/conf.d/sendmail.ini

# Health check
# RUN echo '#!/bin/bash' > /healthcheck && \
# 	echo 'env -i SCRIPT_NAME=/health SCRIPT_FILENAME=/health REQUEST_METHOD=GET cgi-fcgi -bind -connect 127.0.0.1:9000 || exit 1' >> /healthcheck && \
# 	chmod +x /healthcheck

# Clean up
RUN apt-get remove -y git && apt-get autoremove -y && apt-get clean && rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*

EXPOSE 9000
CMD ["php-fpm"]