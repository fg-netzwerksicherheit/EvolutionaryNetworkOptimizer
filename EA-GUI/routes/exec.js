var express = require('express');
var randstr = require('randomstring');
var bodyParser = require('body-parser');
var fs = require('fs-extra');
var path = require('path');
var spawn = require('child_process').spawn;
var router = express.Router();

router.use(bodyParser.json());
router.post('/', function (req, res, next) {
    var id = randstr.generate(8);
    var dir = path.resolve("proc", id);

    // Composing args
    var args = ["run"];

    req.body.id = id;
    var eaml = req.body.eaml;
    if (!eaml) {
        res.status(400).send("EAML is required.");
        return;
    }
    delete req.body.eaml;
    args.push("eaml");

    var variables = req.body.var;
    if (variables) {
        Object.keys(variables).forEach(function (key) {
            var val = variables[key];
            args.push("--" + key + "=" + val);
        });
    }

    var log = req.body.log;
    if (log) {
        args.push("-lt=" + (log.trace || ""));
        args.push("-ld=" + (log.debug || ""));
        args.push("-li=" + (log.info || ""));
        args.push("-le=" + (log.error || ""));
    }

    if (req.body.multithreading)
        args.push("-p" + req.body.multithreading);

    if (req.body.generation)
        args.push("-tg" + req.body.generation);

    if (req.body.evaluation)
        args.push("-te" + req.body.evaluation);

    var backupDir = path.join(dir, "backup");
    args.push("-b" + ((req.body.backup && req.body.backup.frequency) || "") + "=" + backupDir);

    var restore = req.body.restore;
    if (restore) {
        if (!restore.id || restore.id.length != 8) {
            res.status(400).send("Restore ID is required and must be 8 characters long.");
            return;
        }
        var restoreDir = path.resolve("proc", restore.id, "backup");
        if (!fs.existsSync(restoreDir)) {
            res.status(404).send("Restore ID doesn't exist.");
            return;
        }
        args.push("-r" + (restore.from || "") + "=" + restoreDir);
    }

    var port = Math.floor(Math.random() * (65535 - 1024 + 1) + 1024);
    args.push("-s" + port);

    req.body.args = args;
    req.body.time = new Date();

    // Preparation
    fs.mkdirsSync(dir);
    fs.writeFileSync(path.join(dir, "eaml"), eaml);
    fs.writeFileSync(path.join(dir, "config"), JSON.stringify(req.body));

    // Launch
    console.log("\nLaunching proc " + id + " : openea " + args.join(" ") + "\n");
    var ea = spawn("openea", args, {
        cwd: dir
    });

    proc = req.storage.proc;
    proc[id] = {
        stdout: "",
        stderr: "",
        running: true,
        port: port,
        name: req.body.name,
        time: req.body.time,
        handle: ea
    };
    ea.stdout.on('data', function (data) {
        proc[id].stdout += data.toString();
    });
    ea.stderr.on('data', function (data) {
        proc[id].stderr += data.toString();
    });

    // Handle wrong options
    ea.on('exit', function (code) {
        proc[id].running = false;
        if (!res.headersSent && code == 3) {
            res.status(400).send(proc[id].stderr);
            console.log("\nProc " + id + " exited prematurely : " + proc[id].stderr);
        }
        else {
            proc[id].stderr += "\nChild process exited with code " + code.toString() + "\n";
            console.log("\nProc " + id + " exited with code " + code.toString() + "\n");
        }
    });

    setTimeout(function () {
        if (!res.headersSent)
            res.json({
                id: id,
                port: port
            });
    }, 200);
});

module.exports = router;
