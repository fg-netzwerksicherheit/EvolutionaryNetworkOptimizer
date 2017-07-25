var express = require('express');
var path = require('path');
var favicon = require('serve-favicon');
var logger = require('morgan');
var cookieParser = require('cookie-parser');
var bodyParser = require('body-parser');
var fse = require('fs-extra');
var meminfo = require('meminfo');
var index = require('./routes/index');
var exec = require('./routes/exec');
var proc = require('./routes/proc');

var app = express();
var total = 0;
var free = 0;

var storage = {
    proc: {}
};

// uncomment after placing your favicon in /public
//app.use(favicon(path.join(__dirname, 'public', 'favicon.ico')));
app.use(logger('dev'));
app.use(bodyParser.urlencoded({extended: false}));
app.use(cookieParser());

// Persistent storage
app.use(function (req, res, next) {
    req.storage = storage;
    next();
});

app.use('/', index);
app.use('/exec', exec);
app.use('/proc', proc);

app.use(express.static(path.join(__dirname, 'public')));

// catch 404
app.use(function (req, res, next) {
    res.status(404).send("Page not found");
});

// error handler
app.use(function (err, req, res, next) {
    // render the error page
    res.status(err.status || 500);
    res.send(err.message + "<br>" + err.stack);
});

fse.mkdirsSync(path.resolve('proc'));

module.exports = app;
