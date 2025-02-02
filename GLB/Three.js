import * as THREE from 'three';
import { GLTFLoader } from 'three/examples/jsm/loaders/GLTFLoader.js';
import { OrbitControls } from 'three/examples/jsm/controls/OrbitControls.js';

// SCENE 
const scene = new THREE.Scene();
const camera = new THREE.PerspectiveCamera(75, window.innerWidth / window.innerHeight, 0.1, 1000);
camera.position.z = 5;
const renderer = new THREE.WebGLRenderer();
renderer.setSize(window.innerWidth, window.innerHeight);
document.body.appendChild(renderer.domElement);

//LIGHTS
const ambientLight = new THREE.AmbientLight(0x404040); 
scene.add(ambientLight);
const directionalLight = new THREE.DirectionalLight(0xffffff, 1);
directionalLight.position.set(1, 1, 1).normalize();
scene.add(directionalLight);

// MODEL
const loader = new GLTFLoader();
loader.load('C:\GLB\crown.glb', function (gltf) 
{
    scene.add(gltf.scene);
}, 
undefined, function (error) 
{
    console.error(error);
} );

// ANIMATION
function animate()
{
    requestAnimationFrame(animate);
    renderer.render(scene, camera);
}
camera.position.set(0, 1, 5);
animate();

//ORBIT CONTROL
const controls = new OrbitControls(camera, renderer.domElement);
controls.enableDamping = true; // Enable damping (inertia)
controls.dampingFactor = 0.25; // Damping factor
controls.screenSpacePanning = false; // Disable screen space panning
controls.maxPolarAngle = Math.PI / 2; // Limit vertical rotation

function animate() 
{
    requestAnimationFrame(animate);
    controls.update();
    renderer.render(scene, camera);
}
animate();

// CLEANUP
function disposeResources()
{
    scene.traverse(function (object) 
    {
        if (object.geometry) object.geometry.dispose();
        if (object.material) 
        {
            if (object.material.map) object.material.map.dispose();
            object.material.dispose();
        }
    });
}
function removeObjects() 
{
    while (scene.children.length > 0)
    {
        scene.remove(scene.children[0]);
    }
}
disposeResources();
removeObjects();
