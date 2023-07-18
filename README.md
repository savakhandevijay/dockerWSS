# Docker Setup for WSS
The container is crafted to run WSS application on docker without any additional installation.

## Prerequisite
1. [Docker Desktop](https://www.docker.com/products/docker-desktop)
2. [Git](https://git-scm.com/downloads)
3. If using Windows, please also install any [WSL2 distro](https://www.microsoft.com/en-in/p/debian/9msvkqc78pk6#activetab=pivot:overviewtab) of your choice
    - WSL2 distro's I/O works faster with docker than Windows file system due to compatibility

## Steps
1. Edit `.env` if you like to change PHP version, HTTP port for WSS
2. If you make any changes in `.env`, Nginx or PHP configurations, you'll have to take down running containers (`./friday down`) and spin up new containers (`./friday up -d`)

## Try it out
```bash
# Get the docker repository
git clone git@github.com:savakhandevijay/dockerWSS.git

# Initialize the submodule
git submodule init

# Get the WSS repository
git submodule update --recursive --remote

# Spin up the docker container
./friday up -d
```

## Features
The docker container comes with following setup:
1. PHP 7.2 and 7.3 (configurable in `.env` | PHP7.3 is for future use as currently WSS codebase is not compatible with it)
2. Nginx
3. Mailhog

## Friday
The docker repository contains a bash script called `friday` to interact with __PHP__ and __Composer__ containers without any complications.

### PHP
To execute any PHP command or file manually:

```bash
# List all enabled modules
./friday php -m

# Execute any PHP file
./friday php <your-file-name>.php
```

### Composer
```bash
# To install composer dependency
./friday composer install <dependency-name>
```

## Xdebug
As xdebug is a standard and improved way of debugging the code, we have added xdebug support out of the box. Xdebug configurations are stored in `php/configs/xdebug.ini`. Please make changes to it only when you are sure about what you're doing. If you're using Windows 10 Pro with WSL2, then you might have to add new rules to your WSL2's linux distro.

*__Note:__ We've used Xdebug 3 which has different configuration variables. So, if you stuck somewhere while configuring xdebug please check documentation of xdebug 3*

```bash
sudo ufw allow in from 172.16.0.0/16 to any port 9000 comment xDebug9000
sudo ufw allow in from 172.16.0.0/16 to any port 9003 comment xDebug9003
```

To make xdebug work on __VS Code__, please add below configurations to `launch.json` (Menu -> Run -> Open Configurations):
```json
{
    "name": "Listen for XDebug",
    "type": "php",
    "request": "launch",
    "hostname": "0.0.0.0",
    "port": 9003,
    "stopOnEntry": true,
    "log": true,
    "pathMappings": {
        "/var/www/":"${workspaceFolder}/src/www"
    }
},
{
    "name": "Launch currently open script",
    "type": "php",
    "request": "launch",
    "program": "${file}",
    "cwd": "${fileDirname}",
    "port": 9003
}
```

## Virtual host entries
As you must be aware, we need to use a host entry for CWR to properly consume correct WSS endpoints. For this reason, you'll need to map localhost with proper domains. We've provided domain entries for WSS. Please add below entries to your hosts file as per your OS:
```
127.0.0.1 dev-wss2.cex.uk.webuy.com
127.0.0.1 dev-wss2.cex.au.webuy.com
127.0.0.1 dev-wss2.cex.in.webuy.com
127.0.0.1 dev-wss2.cex.pl.webuy.com
127.0.0.1 dev-wss2.cex.pt.webuy.com
127.0.0.1 dev-wss2.cex.es.webuy.com
127.0.0.1 dev-wss2.cex.it.webuy.com
127.0.0.1 dev-wss2.cex.mx.webuy.com
127.0.0.1 dev-wss2.cex.nl.webuy.com
127.0.0.1 dev-wss2.cex.ie.webuy.com
127.0.0.1 dev-wss2.cex.ic.webuy.com
```

As you must have observed from `.env` file or Nginx configurations, for WSS we're using `8080` as port, so please make sure wherever we or our CWR application has used/called WSS endpoints, it's called over `8080` port like `dev-wss2.cex.uk.webuy.test:8080`.

## Enable DBquery profiling
Follow the https://stackoverflow.com/a/36741997/4782382 thread

1. go to src/www/wss2/api/config/cex/main.php
2. add below code under the `components` > `log` array key
```
[
    'class' => 'yii\log\FileTarget',
    'logFile' => '@runtime/logs/profile.log',
    'logVars' => [],
    'levels' => ['profile'],
    'categories' => ['yii\db\Command::query', 'yii\db\Command::execute'],
    'prefix' => function($message) {
        return '';
    }
]
```
3. make sure debug mode enabled either under
**Note** : check `/var/www/wss2/api/web/index.php` configs vars are getting overriding, so highest priority given to `/config/{company}/main.php`
`/var/www/wss2/api/web/index.php`
 or 
 `/var/www/wss2/environments/dev/api/web/index.php`
 4. Enable debug mode & set dev enviorment in any of above file
 ```
defined('YII_DEBUG') or define('YII_DEBUG', true);
defined('YII_ENV') or define('YII_ENV', 'dev');
```
5. file under `log` folder must be writable. e.g `chmod -R 777 log`
6. When you hit any API it will add all the sql's under 
`/var/www/log/wss2-profiling.log` file.