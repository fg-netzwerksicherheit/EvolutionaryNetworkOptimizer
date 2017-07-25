var express = require('express');
var fs = require('fs-extra');
var path = require('path');
var http = require('http');
var router = express.Router();
var meminfo = require('meminfo');

function procList(proc) {
    var procDir = path.resolve("proc");
    var ids = fs.readdirSync(procDir).filter(function (f) {
        return fs.statSync(path.join(procDir, f)).isDirectory() && f.length == 8;
    });
    var info = ids.map(function (id) {
        var result = {
            id: id,
            active: Boolean(proc[id])
        };
        if (result.active) {
            result.running = proc[id].running;
            result.name = proc[id].name;
            result.time = proc[id].time;
        }
        return result;
    });
    return info;
}

// Get list of procs
router.get('/', function (req, res, next) {
    res.json(procList(req.storage.proc));
});

// Get list of active procs
router.get('/active', function (req, res, next) {
    var info = procList(req.storage.proc).filter(function (proc) {
        return proc.active;
    }).sort(function (a, b) {
        return new Date(b.time) - new Date(a.time);
    });
    res.json(info);
});


var idRouter = express.Router({
    mergeParams: true
});
idRouter.use("/", function (req, res, next) {
    var id = req.params.id;
    var dir = path.resolve("proc", id);

    if (!fs.existsSync(dir)) {
        res.status(404).send("Proc ID doesn't exist.");
        return;
    }

    req.procId = id;
    req.procDir = dir;
    req.proc = req.storage.proc[id];

    next();
});

function requireActive(req, res, next) {
    if (!req.proc)
        res.status(400).send("Proc " + req.procId + " is not active.");
    else
        next();
}

// Get status of proc
idRouter.get("/status", requireActive, function (req, res, next) {
    res.json({
        id: req.procId,
        running: req.proc.running,
        port: req.proc.port,
        name: req.proc.name,
        time: req.proc.time,
    });
});

// Forward realtime request
idRouter.use("/realtime", requireActive, function (req, res, next) {
    if (!req.proc.running) {
        res.status(400).send("Proc " + req.procId + " is not running. Cannot access real-time info.");
        return;
    }

    http.get({
        host: 'localhost',
        port: req.proc.port,
        path: req.path,
        method: req.method,
        headers: req.headers
    }, function (response) {
        res.status(response.statusCode);
        res.set(response.headers);
        response.on('data', function (data) {
            res.write(data);
        });
        response.on('end', function () {
            res.end();
        });
    });
});

// Delete proc
idRouter.delete("/", function (req, res, next) {
    if (req.proc && req.proc.running) {
        res.status(400).send("Proc " + req.procId + " is running. Cannot delete.");
        return;
    }
    if (req.proc)
        delete req.storage.proc[req.procId]

    fs.remove(req.procDir);

    res.send("Proc " + req.procId + " has been deleted.");
});

// Get stdout
idRouter.get("/stdout", requireActive, function (req, res, next) {
    var from = parseInt(req.query.from) || 0;
    var to = req.proc.stdout.length;
    var data = req.proc.stdout.slice(from, to);
    res.json({
        next: to,
        data: data
    });
});

// Get stderr
idRouter.get("/stderr", requireActive, function (req, res, next) {
    var from = req.query.from || 0;
    var to = req.proc.stderr.length;
    var data = req.proc.stderr.slice(from, to);
    res.json({
        next: to,
        data: data
    });
});

// Terminate proc
idRouter.post("/term", requireActive, function (req, res, next) {
    req.proc.handle.kill("SIGTERM");
    res.send("SIGTERM sent.");
});

// Get EAML
idRouter.get("/eaml", function (req, res, next) {
    res.sendFile(path.resolve(req.procDir, "eaml"), {
        headers: {
            "Content-Type": "application/xml"
        }
    });
});

// Get config
idRouter.get("/config", function (req, res, next) {
    res.sendFile(path.resolve(req.procDir, "config"), {
        headers: {
            "Content-Type": "application/json"
        }
    });
});

router.use("/:id([a-zA-Z0-9]{8})", idRouter);
module.exports = router;
