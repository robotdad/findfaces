const express = require('express');
const app = express();
const path = require('path');

const router = express.Router();
const upload = require('./uploadMiddleware');
const fileUpload = require('./fileUpload');
const request = require('request');
var fs = require('fs');

router.get('/', async function (req, res) {
    await res.render('index');
});

router.post('/post', upload.single('image'), async function (req, res) {
    const imagePath = path.join(__dirname, '/public/images');
    const imageFileUpload = new fileUpload(imagePath);
    if (!req.file) {
        res.status(401).json({ error: 'Please provide an image' });
    }
    const filename = await imageFileUpload.save(req.file.buffer);
    var facesurl = 'http://findfaces-svc:4000/files?submit=' + filename;
    console.log("facesurl: " + facesurl)
    var putImage = imagePath + path.sep + filename;
    console.log("putImage: " + putImage);

    request({
        method: 'PUT',
        uri: facesurl,
        encoding: null,
        body: fs.createReadStream(putImage)
        },
        function (error, response, body) {
            if (error) {
                return console.error('upload failed:', error);
            }
            console.log('Upload successful!  Server responded with:', body);
    });

    //return res.status(200).json({ name: filename });
    return res.render('image', {image: filename});
});

router.get('/image', async function (req, res) {
    let image = req.query.image
    await res.render('image');
});

module.exports = router;