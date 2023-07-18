listen 80;
listen  443 ssl http2;
listen  [::]:443 ssl http2;
ssl_certificate /etc/nginx/ssl/selfsigned/selfsigned.crt;
ssl_certificate_key /etc/nginx/ssl/selfsigned/selfsigned.key;

root /var/www/wss2/api/web;
# access_log /var/log/wss/nginx-access.log main;
index index.html index.htm index.php;

charset utf-8;

add_header X-Frame-Options DENY;
add_header X-XSS-Protection "1; mode=block";
add_header X-Content-Type-Options nosniff;
add_header Access-Control-Allow-Origin * always;

set $yii_bootstrap "index.php";

client_max_body_size 52m;

location = /favicon.ico {
    log_not_found off; access_log off;
}
location = /robots.txt {
    log_not_found off; access_log off;
}

location / {
    # CORS-for-CWR
    if ($request_method = 'OPTIONS') {
        add_header Access-Control-Allow-Origin * always;
        add_header Access-Control-Allow-Methods 'GET, POST, PUT, DELETE, OPTIONS';
        add_header Access-Control-Allow-Headers 'DNT,X-CustomHeader,Keep-Alive,User-Agent,X-Requested-With,If-Modified-Since,Cache-Control,Content-Type,Content-Range,Range';
        return 204;
    }

    index  index.html $yii_bootstrap;
    try_files $uri $uri/ /$yii_bootstrap?$args;
}

location ~ ^/(protected|framework|themes/\w+/views) {
    deny  all;
}

location /docs/ {
    index index.php;
}

location ~ ^/docs/api-json/.*\.json$ {
    rewrite  ^/docs/api-json/(.*\.json)(/?)$ /docs/swagger.view.php?file=$1&version=$arg_version;
}

location ~ \.php$ {
    fastcgi_split_path_info  ^(.+\.php)(.*)$;

    #let yii catch the calls to unexising PHP files
    set $fsn /$yii_bootstrap;
    if (-f $document_root$fastcgi_script_name){
        set $fsn $fastcgi_script_name;
    }
    
    fastcgi_pass wss:9000;
    fastcgi_index index.php;
    include fastcgi_params;
    fastcgi_param SCRIPT_FILENAME $document_root$fastcgi_script_name;
    fastcgi_param PATH_INFO $fastcgi_path_info;
}