describe('Example', () => {
  it('Launch SimpleViewApp', async () => {
    //await expect(element(by.id('welcome'))).toBeVisible();
    console.log("test - launch app");
    await device.launchApp();
    await new Promise(resolve => setTimeout(resolve, 60000)); 	  
  });

});
