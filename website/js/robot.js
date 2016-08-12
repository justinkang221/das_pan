$(document).ready(function () {
    var loaded = false;

    const LEFT = -1, CENTER = 0, RIGHT = 1;
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
    const light = new THREE.PointLight();
    light.position.set(0, 1000, 0);
    scene.add(light);

    const texture = new THREE.TextureLoader();
    var geometry = new THREE.SphereGeometry(5000, 60, 40);
    var uniforms = {
        texture: {type: 't', value: texture.load('img/texture/sky.jpg')}
    };

    var material = new THREE.ShaderMaterial({
        uniforms: uniforms,
        vertexShader: document.getElementById('sky-vertex').textContent,
        fragmentShader: document.getElementById('sky-fragment').textContent
    });

    var sky = new THREE.Mesh(geometry, material);
    sky.scale.set(-1, 1, 1);
    sky.rotation.order = 'XZY';
    sky.renderOrder = 1000.0;
    scene.add(sky);

    var translation, rotation, transformation;

    var chassis, tinah;
    var wheels = new THREE.Object3D();
    var pan = {left: 0, right: 0};
    var admiral, bar, broom;
    var forearm = new THREE.Object3D();
    var arm = new THREE.Object3D();
    var robot = new THREE.Object3D();

    const loader = new THREE.STLLoader();
    loader.load('stl/chassis.stl', function (geometry) {
        material = new THREE.MeshPhongMaterial({color: 0x555555});
        translation = new THREE.Matrix4().makeTranslation(-100, 0, -150);
        rotation = new THREE.Matrix4().makeRotationY(Math.PI / 2);
        transformation = new THREE.Matrix4().multiplyMatrices(rotation, translation);
        geometry.applyMatrix(transformation);
        chassis = new THREE.Mesh(geometry, material);
        robot.add(chassis);
    });

    loader.load('stl/tinah.stl', function (geometry) {
        material = new THREE.MeshPhongMaterial({color: 0x3A6629});
        translation = new THREE.Matrix4().makeTranslation(-60, 70, 100);
        rotation = new THREE.Matrix4().makeRotationY(Math.PI);
        transformation = new THREE.Matrix4().multiplyMatrices(rotation, translation);
        geometry.applyMatrix(transformation);
        tinah = new THREE.Mesh(geometry, material);
        robot.add(tinah);
    });

    loader.load('stl/wheel.stl', function (geometry) {
        material = new THREE.MeshPhongMaterial({color: 0xFFFF00});
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

        translation = new THREE.Matrix4().makeTranslation(50, 20, -155);
        wheels.applyMatrix(translation);

        robot.add(wheels);
    });

    loader.load('stl/pan.stl', function (geometry) {
        material = new THREE.MeshPhongMaterial({color: 0x66CCCC});
        translation = new THREE.Matrix4().makeTranslation(-110, 0, 50);
        rotation = new THREE.Matrix4().makeRotationY(0);
        transformation = new THREE.Matrix4().multiplyMatrices(rotation, translation);
        geometry.applyMatrix(transformation);
        pan.left = new THREE.Mesh(geometry, material);

        translation = new THREE.Matrix4().makeTranslation(150, 13, -150);
        pan.left.applyMatrix(translation);
        robot.add(pan.left);

        geometry = geometry.clone();
        geometry.applyMatrix(new THREE.Matrix4().getInverse(transformation));
        translation = new THREE.Matrix4().makeTranslation(-110, 0, 50);
        rotation = new THREE.Matrix4().makeRotationY(Math.PI);
        transformation = new THREE.Matrix4().multiplyMatrices(rotation, translation);
        geometry.applyMatrix(transformation);
        pan.right = new THREE.Mesh(geometry, material);

        translation = new THREE.Matrix4().makeTranslation(-147, 13, 147);
        pan.right.applyMatrix(translation);
        robot.add(pan.right);

        loaded = true;
    });

    loader.load('stl/admiral.stl', function (geometry) {
        material = new THREE.MeshPhongMaterial({color: 0xE9C2A6});
        translation = new THREE.Matrix4().makeTranslation(-12, 120, 12);
        rotation = new THREE.Matrix4().makeRotationY(Math.PI / 2);
        transformation = new THREE.Matrix4().multiplyMatrices(translation, rotation);
        geometry.applyMatrix(transformation);
        admiral = new THREE.Mesh(geometry, material);

        arm.add(admiral);
    });

    loader.load('stl/bar.stl', function (geometry) {
        material = new THREE.MeshPhongMaterial({color: 0xfcfcfc, transparent: true, opacity: 0.2});
        translation = new THREE.Matrix4().makeTranslation(-16, -10, 0);
        rotation = new THREE.Matrix4().makeRotationZ(-Math.PI / 12);
        transformation = new THREE.Matrix4().multiplyMatrices(translation, rotation);
        geometry.applyMatrix(transformation);
        bar = new THREE.Mesh(geometry, material);

        forearm.add(bar);
    });

    loader.load('stl/broom.stl', function (geometry) {
        material = new THREE.MeshPhongMaterial({color: 0xfafafa, specular: 0xfdfdfd, shininess: 100});
        translation = new THREE.Matrix4().makeTranslation(-110, -165, 0);
        rotation = new THREE.Matrix4().makeRotationZ(-Math.PI / 6);
        transformation = new THREE.Matrix4().multiplyMatrices(translation, rotation);
        geometry.applyMatrix(transformation);
        broom = new THREE.Mesh(geometry, material);

        translation = new THREE.Matrix4().makeTranslation(220, -65, -60);
        broom.applyMatrix(translation);
        forearm.add(broom);
    });

    translation = new THREE.Matrix4().makeTranslation(0, 427, -3);
    rotation = new THREE.Matrix4().makeRotationY(Math.PI / 2);
    transformation = new THREE.Matrix4().multiplyMatrices(translation, rotation);
    forearm.applyMatrix(transformation);
    arm.add(forearm);
    translation = new THREE.Matrix4().makeTranslation(0, 0, 5);
    arm.applyMatrix(translation);
    robot.add(arm);

    scene.add(robot);

    const bounds = {
        x: 4000,
        z: 2000
    };

    const plane = new THREE.PlaneGeometry(2 * (bounds.x + 100), 2 * (bounds.z + 100));
    const ground = new THREE.Mesh(plane, new THREE.MeshBasicMaterial({map: texture.load('img/texture/map.png')}));
    ground.receiveShadow = true;
    ground.rotation.x = -Math.PI / 2;
    ground.position.set(0, -14, 0);
    scene.add(ground);

    camera.up.set(0, 1, 0);
    camera.position.set(0, 600, -bounds.z);

    var location = {
        x: 0,
        z: 0
    };
    var speed = 20;
    var sweep = {
        top: Math.PI / 6,
        rest: 0,
        bottom: -Math.PI / 6,
        step: Math.PI / 48,
        stage: 0
    };
    var brush = Math.PI / 24;

    const controls = {
        left: false,
        up: false,
        right: false,
        down: false,
        pan: {
            left: true,
            right: true
        },
        arm: {
            orient: CENTER,
            sweep: false,
            brush: false
        }
    };

    $(canvas).keydown(function (e) {
        var prevent = true;
        controls.move = true;
        switch (e.keyCode) {
            case 37:
                controls.left = true;
                if (!controls.pan.left) {
                    controls.pan.left = true;
                    controls.arm.orient = CENTER;
                }
                if (!controls.pan.right) {
                    controls.pan.right = true;
                    controls.arm.orient = CENTER;
                }
                break;
            case 38:
                controls.up = true;
                if (!controls.pan.left) {
                    controls.pan.left = true;
                    controls.arm.orient = CENTER;
                }
                if (!controls.pan.right) {
                    controls.pan.right = true;
                    controls.arm.orient = CENTER;
                }
                break;
            case 39:
                controls.right = true;
                if (!controls.pan.left) {
                    controls.pan.left = true;
                    controls.arm.orient = CENTER;
                }
                if (!controls.pan.right) {
                    controls.pan.right = true;
                    controls.arm.orient = CENTER;
                }
                break;
            case 40:
                controls.down = true;
                if (!controls.pan.left) {
                    controls.pan.left = true;
                    controls.arm.orient = CENTER;
                }
                if (!controls.pan.right) {
                    controls.pan.right = true;
                    controls.arm.orient = CENTER;
                }
                break;
            case 88:
            case 90:
            case 32:
                controls.move = false;
                prevent = true;
                break;
            default:
                controls.move = false;
                prevent = false;
        }
        if (prevent) e.preventDefault();
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
            case 90:
                controls.pan.left = !controls.pan.left;
                if (!controls.pan.right) controls.pan.right = true;
                if (!controls.pan.left) controls.arm.orient = LEFT;
                else if (controls.pan.right) controls.arm.orient = CENTER;
                break;
            case 88:
                controls.pan.right = !controls.pan.right;
                if (!controls.pan.left) controls.pan.left = true;
                if (!controls.pan.right) controls.arm.orient = RIGHT;
                else if (controls.pan.left) controls.arm.orient = CENTER;
                break;
            case 32:
                controls.arm.sweep = true;
                controls.arm.sweep = !(controls.pan.left && controls.pan.right);
                controls.arm.brush = controls.arm.sweep;
                break;
            default:
                prevent = false;
        }
        if (prevent) e.preventDefault();
    });

    function render() {
        requestAnimationFrame(render);

        camera.position.set(0.3 * robot.position.x, robot.position.y + 1000 - 0.3 * robot.position.z, 0.3 * robot.position.z - 800);
        camera.lookAt(robot.position);

        if (controls.left) {
            robot.rotation.y += Math.PI / 48;
            robot.rotation.y %= 2 * Math.PI;
            location.x = robot.position.x + speed * Math.sin(robot.rotation.y);
            location.z = robot.position.z + speed * Math.cos(robot.rotation.y);
            wheels.rotation.x += (Math.abs(location.x) > bounds.x || Math.abs(location.z) > bounds.z) ? Math.PI / 3 : Math.PI / 24;
        }
        if (controls.right) {
            robot.rotation.y -= Math.PI / 48;
            robot.rotation.y %= 2 * Math.PI;
            location.x = robot.position.x + speed * Math.sin(robot.rotation.y);
            location.z = robot.position.z + speed * Math.cos(robot.rotation.y);
            wheels.rotation.x += (Math.abs(location.x) > bounds.x || Math.abs(location.z) > bounds.z) ? Math.PI / 3 : Math.PI / 24;
        }

        if (controls.up) {
            location.x = robot.position.x + speed * Math.sin(robot.rotation.y);
            location.z = robot.position.z + speed * Math.cos(robot.rotation.y);
            wheels.rotation.x += (Math.abs(location.x) > bounds.x || Math.abs(location.z) > bounds.z) ? Math.PI / 3 : Math.PI / 24;
        }

        if (controls.down) {
            location.x = robot.position.x - speed * Math.sin(robot.rotation.y);
            location.z = robot.position.z - speed * Math.cos(robot.rotation.y);
            wheels.rotation.x += (Math.abs(location.x) > bounds.x || Math.abs(location.z) > bounds.z) ? Math.PI / 3 : Math.PI / 24;
        }

        robot.position.x = Math.max(Math.min(location.x, bounds.x), -1 * bounds.x);
        robot.position.z = Math.max(Math.min(location.z, bounds.z), -1 * bounds.z);


        if (controls.pan.left) {
            pan.left.rotation.z = Math.min(pan.left.rotation.z + Math.PI / 24, 0);
        }
        else {
            pan.left.rotation.z = Math.max(pan.left.rotation.z - Math.PI / 24, -Math.PI / 2);
        }

        if (controls.pan.right) {
            pan.right.rotation.z = Math.max(pan.right.rotation.z - Math.PI / 24, 0);
        }
        else {
            pan.right.rotation.z = Math.min(pan.right.rotation.z + Math.PI / 24, Math.PI / 2);
        }

        switch (controls.arm.orient) {
            case LEFT:
                arm.rotation.y = Math.max(arm.rotation.y - Math.PI / 24, -Math.PI / 2);
                break;
            case RIGHT:
                arm.rotation.y = Math.min(arm.rotation.y + Math.PI / 24, Math.PI / 2);
                break;
            case CENTER:
                if (arm.rotation.y > 0) arm.rotation.y = Math.max(arm.rotation.y - Math.PI / 24, 0);
                else arm.rotation.y = Math.min(arm.rotation.y + Math.PI / 24, 0);
                break;
        }

        if (controls.arm.sweep) {
            switch (sweep.stage) {
                case 0:
                    forearm.rotation.z = Math.min(sweep.top, forearm.rotation.z + sweep.step);
                    if (forearm.rotation.z == sweep.top) {
                        sweep.stage = 1;
                    }
                    break;
                case 1:
                    forearm.rotation.z = Math.max(sweep.bottom, forearm.rotation.z - sweep.step);
                    if (forearm.rotation.z == sweep.bottom) {
                        sweep.stage = 2;
                    }
                    break;
                case 2:
                    forearm.rotation.z = Math.min(sweep.rest, forearm.rotation.z + sweep.step);
                    if (forearm.rotation.z == sweep.rest) {
                        sweep.stage = 0;
                        controls.arm.sweep = false;
                    }
                    break;
            }
        }

        if (controls.arm.brush) {
            broom.rotation.z = Math.min(2 * Math.PI / 3, Math.max(0, broom.rotation.z + brush));
            if (broom.rotation.z == 2 * Math.PI / 3) {
                brush = -Math.PI / 24;
            }
            else if (broom.rotation.z == 0) {
                controls.arm.brush = false;
                brush = Math.PI / 24;
            }
        }

        renderer.render(scene, camera);
    }

    var interval = setInterval(function () {
        if (loaded) {
            render();
            clearInterval(interval);
        }
    }, 10);

});
