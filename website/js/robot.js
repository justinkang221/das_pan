$(document).ready(function () {
    const canvas = document.getElementById('webGL-canvas');
    const scene = new THREE.Scene();
    const camera = new THREE.PerspectiveCamera(75, canvas.clientWidth / canvas.clientHeight, 0.1, 10000);

    const renderer = new THREE.WebGLRenderer({antialias: false});
    renderer.setSize(canvas.clientWidth, canvas.clientHeight);
    renderer.setClearColor(0xfafafa);
    renderer.setPixelRatio(window.devicePixelRatio);
    canvas.appendChild(renderer.domElement);

    window.addEventListener('resize', function () {
            camera.aspect = canvas.clientWidth / canvas.clientHeight;
            camera.updateProjectionMatrix();

            renderer.setSize(canvas.clientWidth, canvas.clientHeight);
        }
    );

    const ambient = new THREE.AmbientLight(0x404040);
    scene.add(ambient);
    const pointLight = new THREE.PointLight();
    pointLight.position.set(-256, 256, -256);
    scene.add(pointLight);

    var translation, rotation, scale, transformation;
    var material, chassis;
    var wheels = new THREE.Object3D();
    var pan = {left: 0, right: 0};
    var robot = new THREE.Object3D();

    const loader = new THREE.STLLoader();
    loader.load('stl/chassis.stl', function (geometry) {
        material = new THREE.MeshPhongMaterial({color: 0xfafafa});
        translation = new THREE.Matrix4().makeTranslation(-250, 0, -150);
        rotation = new THREE.Matrix4().makeRotationY(Math.PI / 2);
        transformation = new THREE.Matrix4().multiplyMatrices(rotation, translation);
        geometry.applyMatrix(transformation);
        chassis = new THREE.Mesh(geometry, material);
        robot.add(chassis);
    });

    loader.load('stl/wheel.stl', function (geometry) {
        material = new THREE.MeshPhongMaterial({color: 0xfafafa});
        translation = new THREE.Matrix4().makeTranslation(-38, -38, -200);
        rotation = new THREE.Matrix4().makeRotationY(Math.PI / 2);
        transformation = new THREE.Matrix4().multiplyMatrices(rotation, translation);
        geometry.applyMatrix(transformation);
        wheels.add(new THREE.Mesh(geometry, material));

        geometry = geometry.clone();
        geometry.applyMatrix(new THREE.Matrix4().getInverse(transformation));
        translation = new THREE.Matrix4().makeTranslation(-38, -38, 80);
        rotation = new THREE.Matrix4().makeRotationY(Math.PI / 2);
        transformation = new THREE.Matrix4().multiplyMatrices(rotation, translation);
        geometry.applyMatrix(transformation);
        wheels.add(new THREE.Mesh(geometry, material));

        translation = new THREE.Matrix4().makeTranslation(50, 20, -5);
        wheels.applyMatrix(translation);

        robot.add(wheels);
    });

    loader.load('stl/pan.stl', function (geometry) {
        material = new THREE.MeshPhongMaterial({color: 0xfafafa});
        translation = new THREE.Matrix4().makeTranslation(-110, 0, 50);
        rotation = new THREE.Matrix4().makeRotationY(0);
        transformation = new THREE.Matrix4().multiplyMatrices(rotation, translation);
        geometry.applyMatrix(transformation);
        pan.left = new THREE.Mesh(geometry, material);

        translation = new THREE.Matrix4().makeTranslation(150, 5, 0);
        pan.left.applyMatrix(translation);
        robot.add(pan.left);

        geometry = geometry.clone();
        geometry.applyMatrix(new THREE.Matrix4().getInverse(transformation));
        translation = new THREE.Matrix4().makeTranslation(-110, 0, 50);
        rotation = new THREE.Matrix4().makeRotationY(Math.PI);
        transformation = new THREE.Matrix4().multiplyMatrices(rotation, translation);
        geometry.applyMatrix(transformation);
        pan.right = new THREE.Mesh(geometry, material);

        translation = new THREE.Matrix4().makeTranslation(-147, 5, 297);
        pan.right.applyMatrix(translation);
        robot.add(pan.right);
    });

    scene.add(robot);

    const bounds = {x: 1000, z: 700};

    const plane = new THREE.PlaneGeometry(2 * (bounds.x + 220), 2 * (bounds.z + 240));
    const ground = new THREE.Mesh(plane, new THREE.MeshBasicMaterial({color: 0xaaaaaa}));
    ground.receiveShadow = true;
    ground.rotation.x = -Math.PI / 2;
    ground.position.set(0, -50, 0);
    scene.add(ground);

    camera.up.set(0, 1, 0);
    camera.position.set(0, 0, 5);

    var angle, difference;
    var location = {x: 0, z: 0};

    const controls = {
        left: false,
        up: false,
        right: false,
        down: false,
        pan: {
            left: true,
            right: true
        }
    };

    const direction = new THREE.Vector3(0, 0, 0);

    function setDirection(controls) {
        var x = controls.left ? 1 : controls.right ? -1 : 0,
            y = 0,
            z = controls.up ? 1 : controls.down ? -1 : 0;
        direction.set(x, y, z);
    }

    $(canvas).keydown(function (e) {
        var prevent = true;
        switch (e.keyCode) {
            case 37:
                controls.left = true;
                break;
            case 38:
                controls.up = true;
                break;
            case 39:
                controls.right = true;
                break;
            case 40:
                controls.down = true;
                break;
            default:
                prevent = false;
        }
        if (prevent) e.preventDefault();
        setDirection(controls);
    });

    $(canvas).keyup(function (e) {
        var prevent = true;
        switch (e.keyCode) {
            case 37:
                controls.left = false;
                break;
            case 38:
                controls.up = false;
                break;
            case 39:
                controls.right = false;
                break;
            case 40:
                controls.down = false;
                break;
            case 81:
                controls.pan.left = !controls.pan.left;
                if (!controls.pan.right) controls.pan.right = true;
                break;
            case 69:
                controls.pan.right = !controls.pan.right;
                if (!controls.pan.left) controls.pan.left = true;
                break;
            default:
                prevent = false;
        }
        if (prevent) e.preventDefault();
        setDirection(controls);
    });

    function render() {
        requestAnimationFrame(render);

        camera.position.set(robot.position.x, robot.position.y + 500, -800);
        camera.lookAt(robot.position);

        if (direction.x || direction.z) {
            robot.rotation.y %= 2 * Math.PI;
            angle = Math.atan2(direction.x, direction.z);
            difference = angle - robot.rotation.y;

            difference += (Math.abs(difference) > Math.PI)
                * ((difference > 0) * (-2 * Math.PI) + (difference < 0) * (2 * Math.PI));

            robot.rotation.y += (difference !== 0) * difference / 7;

            location.x = robot.position.x + direction.x * ((direction.z === 0) ? 10 : Math.sqrt(20));
            location.z = robot.position.z + direction.z * ((direction.x === 0) ? 10 : Math.sqrt(20));

            robot.position.x = Math.max(Math.min(location.x, bounds.x), -1 * bounds.x);
            robot.position.z = Math.max(Math.min(location.z, bounds.z), -1 * bounds.z);

            wheels.rotation.x += (Math.abs(location.x) > bounds.x || Math.abs(location.z) > bounds.z) ? Math.PI / 3 : Math.PI / 24;

            if (!controls.pan.left) controls.pan.left = true;
            if (!controls.pan.right) controls.pan.right = true;
        }

        if (controls.pan.left) {
            pan.left.rotation.z = Math.min(pan.left.rotation.z + Math.PI/24, 0);
        }
        else {
            pan.left.rotation.z = Math.max(pan.left.rotation.z - Math.PI/24, -Math.PI/2)
        }

        if (controls.pan.right) {
            pan.right.rotation.z = Math.max(pan.right.rotation.z - Math.PI/24, 0);
        }
        else {
            pan.right.rotation.z = Math.min(pan.right.rotation.z + Math.PI/24, Math.PI/2);
        }

        renderer.render(scene, camera);
    }

    render();
});
