'use strict';

var express = require('express');
var router = express.Router();

/* GET home page. */
router.get('/', function(req, res, next) {
  res.render('index', { title: 'Chart me!' });
});

router.get('/options', function(req, res, next) {
    var app = require('../app');
    res.setHeader('Content-Type', 'application/json');
    res.send(JSON.stringify(app.get('options')));
});

module.exports = router;
